#!/usr/bin/env python3

import re
import argparse
import json
import os
import sys
import time
import logging
import humanize

from types import SimpleNamespace
from http.client import RemoteDisconnected
from requests.exceptions import ConnectionError

from cdrouter import CDRouter
from cdrouter.jobs import Job, Options
from cdrouter.configs import Config
from cdrouter.cdr_error import CDRouterError


class OpenWrtSystemInfo:
    def __init__(self, filename):
        with open(filename, "r") as read_file:
            self.data = json.load(read_file)

    @property
    def kernel(self):
        return self.data["kernel"]

    @property
    def board_name(self):
        board_name = self.data["board_name"]
        fixup = {
            "EASY350 ANYWAN (GRX350) Axepoint Asurada model": "nec-wx3000hp",
            "EASY350 ANYWAN (GRX350) Main model": "nec-wx3000hp",
        }
        board_name = fixup.get(board_name, board_name)
        return board_name.replace(",", "-")

    @property
    def target(self):
        return self.data["release"]["target"]

    @property
    def distribution(self):
        return self.data["release"]["distribution"]

    @property
    def revision(self):
        return self.data["release"]["revision"]

    @property
    def version(self):
        return self.data["release"]["version"]

    def sanitize(self, s):
        return s.replace("+", "-").replace("/", "-")

    def as_tags(self):
        return self.sanitize(
            "kernel_{},board_{},target_{},revision_{},version_{},distro_{}".format(
                self.kernel,
                self.board_name,
                self.target,
                self.revision,
                self.version,
                self.distribution,
            )
        )


class TestbedCDRouter:
    def __init__(self, args):
        self.args = args
        self.configs_path = os.path.join(self.args.root_dir, "configurations")
        self.packages_path = os.path.join(self.args.root_dir, "packages")

    def connect(self):
        api_token = os.getenv("CDROUTER_API_TOKEN")
        api_url = os.getenv("CDROUTER_API_URL")

        if not api_token or not api_url:
            logging.error("API token or URL is missing")
            exit(1)

        logging.debug("Connecting to {}".format(api_url))
        self.cdr = CDRouter(api_url, token=api_token)

    def job_launch(self):
        p = self.cdr.packages.get_by_name(self.args.package_name)
        logging.info(
            "Using test package '{name}' with id {id} and {test_count} tests defined.".format(
                name=p.name, id=p.id, test_count=p.test_count
            )
        )

        p = self.cdr.packages.get(p.id)
        config = self.cdr.configs.get_by_name(self.args.configuration)
        device = self.cdr.devices.get_by_name(self.args.device)
        if config:
            p.config_id = config.id
        if device:
            p.device_id = device.id
        if config or device:
            self.cdr.packages.edit(p)

        a = self.cdr.packages.analyze(p.id)
        self.job_total = a.run_count
        logging.info(
            "Package '{name}' will run {run_count} tests and skip {skipped_count} tests.".format(
                name=p.name, run_count=a.run_count, skipped_count=a.skipped_count
            )
        )

        for test in a.skipped_tests:
            logging.info(
                "Skipping test '{synopsis}' due to '{skip_name}'.".format(
                    synopsis=test.synopsis, skip_name=test.skip_name
                )
            )

        tags = None
        if self.args.system_info:
            tags = OpenWrtSystemInfo(self.args.system_info).as_tags().split(",")

        if self.args.tags:
            tags += self.args.tags.split(",")

        options = Options(tags=tags)
        job = Job(package_id=p.id, options=options)
        job = self.cdr.jobs.launch(job)
        while job.result_id is None:
            time.sleep(1)
            job = self.cdr.jobs.get(job.id)

        self.job = job

    def job_progress(self):
        current = None
        job = self.cdr.jobs.get(self.job.id)
        while job.status == "running":
            updates = self.cdr.results.updates(job.result_id)
            running = updates.running
            progress = updates.progress

            if not running or not progress:
                job = self.cdr.jobs.get(self.job.id)
                continue

            description = running.description
            if current == description:
                job = self.cdr.jobs.get(self.job.id)
                continue

            current = description

            logging.info(
                "Running test {finished}/{total} ({percent}%) '{description}'".format(
                    description=description,
                    percent=progress.progress,
                    finished=progress.finished,
                    total=self.job_total,
                )
            )

            time.sleep(5)
            job = self.cdr.jobs.get(self.job.id)

    def job_result(self, job_id=None):
        job_id = job_id or self.job.result_id
        r = self.cdr.results.get(job_id)

        buf, filename = self.cdr.results.download_logdir_archive(job_id, format="tgz")
        logging.info("Exporting logdir archive {}".format(filename))
        with open(filename, "wb") as f:
            f.write(buf.getvalue())

        logging.info(
            "Test job finished as '{}' after {}.".format(
                r.status, humanize.naturaldelta(r.duration)
            )
        )
        logging.info(
            "Run {} tests, which {} failed and {} passed.".format(
                r.tests, r.fail, r.passed
            )
        )

        if r.fail == 0 and r.status == "completed":
            logging.info("Success!")
            exit(0)

        if r.fail == 0:
            exit(1)

        logging.error("{:=^50}".format(" [ FAILED TESTS ] "))
        for test in self.cdr.tests.iter_list(r.id, filter=["result=fail"]):
            logging.error("{} ({})".format(test.description, test.result))

        exit(1)

    def package_run(self):
        self.connect()
        self.job_launch()
        self.job_progress()
        self.job_result()

    def netif_available(self, name):
        for netif in self.cdr.system.interfaces():
            if netif.name == name:
                return True

        return False

    def wait_for_netif(self):
        self.connect()

        name = self.args.name
        timeout = time.time() + self.args.timeout

        while True:
            if self.netif_available(name):
                logging.info("Interface {} is available.".format(name))
                exit(0)

            if time.time() > timeout:
                logging.error(
                    "Interface {} is not available after {}".format(
                        name, humanize.naturaldelta(self.args.timeout)
                    )
                )
                exit(1)

            time.sleep(5)

    def package_stop(self):
        self.connect()

        f = ["status~(running|paused)"]
        pkg_name = self.args.package_name
        if pkg_name:
            f.append("package_name={}".format(pkg_name))

        for r in self.cdr.results.iter_list(filter=f):
            self.cdr.results.stop(r.id)
            logging.info(
                "Stopped '{}' package which was {}".format(r.package_name, r.status)
            )

    def package_export(self):
        self.connect()
        name = self.args.name
        p = self.cdr.packages.get_by_name(name)
        buf, filename = self.cdr.packages.export(p.id)
        filename = "{}.gz".format(self.args.filename or name)
        self.file_save(self.packages_path, buf.getvalue(), filename)

    def package_import(self):
        self.connect()
        archive = os.path.join(self.packages_path, self.args.filename)

        with open(archive, "rb+") as fd:
            si = self.cdr.imports.stage_import_from_file(fd)

        filename = re.sub(r"\.gz$", "", self.args.filename)
        name = self.args.name or filename

        req = self.cdr.imports.get_commit_request(si.id)
        for id in req.packages:
            p = req.packages[id]
            p.name = name
            logging.debug("Going to import package '{}'".format(name))
            p.should_import = True

        resp = self.cdr.imports.commit(si.id, req)
        for id in resp.packages:
            r = resp.packages[id].response
            if not r.imported:
                logging.error("Import of '{}' failed: {}".format(r.name, r.message))
                exit(1)

        logging.info(
            "Imported package '{}' from '{}'".format(r.name, self.args.filename)
        )

    def file_save(self, path, content, filename=None):
        filename = filename or self.args.filename
        os.makedirs(path, exist_ok=True)
        dest = os.path.join(path, filename)
        with open(dest, "wb") as f:
            f.write(content)

    def config_export(self):
        self.connect()
        c = self.cdr.configs.get_by_name(self.args.name)
        content = self.cdr.configs.get_plaintext(c.id)
        self.file_save(self.configs_path, content.encode())

    def file_content(self, path, filename=None):
        content = None
        filename = filename or self.args.filename
        src = os.path.join(path, filename)
        with open(src, "rb") as f:
            content = f.read()
        return content

    def config_check(self):
        self.connect()
        content = self.file_content(self.configs_path)
        check = self.cdr.configs.check_config(content)
        if not check.errors:
            logging.info("OK, no errors!")
            exit(0)

        for error in check.errors:
            logging.error(
                "{}: {}: {}".format(
                    self.args.filename, ",".join(error.lines), error.error
                )
            )

        exit(1)

    def config_import(self):
        self.connect()
        name = self.args.name or self.args.filename

        try:
            c = self.cdr.configs.get_by_name(name)
            self.cdr.configs.delete(c.id)
            logging.debug("Deleted already existing config '{}'".format(c.name))
        except CDRouterError:
            pass

        content = self.file_content(self.configs_path)
        config = Config(contents=content, name=name)
        self.cdr.configs.create(config)
        logging.info("Imported config '{}' from '{}'".format(name, self.args.filename))


def main():
    logging.basicConfig(
        level=logging.INFO, format="%(levelname)7s: %(message)s", stream=sys.stderr
    )

    parser = argparse.ArgumentParser()
    parser.add_argument(
        "-r",
        "--root-dir",
        type=str,
        default=os.environ.get("TB_CDROUTER_ROOT", ".testbed/cdrouter"),
        help="CDRouter root directory (default: %(default)s)",
    )
    parser.add_argument("-d", "--debug", action="store_true", help="enable debug mode")

    subparsers = parser.add_subparsers(dest="command", title="available subcommands")
    subparser = subparsers.add_parser("package_run", help="run package")
    subparser.add_argument("package_name", help="name of the testing package")
    subparser.add_argument("-t", "--tags", help="additional tags for the result")
    subparser.add_argument("-d", "--device", help="device used for testing")
    subparser.add_argument(
        "-c", "--configuration", help="configuration used for testing"
    )
    subparser.add_argument(
        "-s",
        "--system-info",
        help="JSON file with system information for additional tags",
    )
    subparser.set_defaults(func=TestbedCDRouter.package_run)

    subparser = subparsers.add_parser(
        "package_stop", help="stop running/paused package"
    )
    subparser.add_argument(
        "-p", "--package-name", help="name of the package (default: any)"
    )
    subparser.set_defaults(func=TestbedCDRouter.package_stop)

    subparser = subparsers.add_parser("package_export", help="export package")
    subparser.add_argument("name", help="package name")
    subparser.add_argument(
        "-f", "--filename", help="destination filename (default: name)"
    )
    subparser.set_defaults(func=TestbedCDRouter.package_export)

    subparser = subparsers.add_parser("package_import", help="package configuration")
    subparser.add_argument("filename", help="package filename")
    subparser.add_argument("-n", "--name", help="package name, (default: filename)")
    subparser.set_defaults(func=TestbedCDRouter.package_import)

    subparser = subparsers.add_parser("config_export", help="export configuration")
    subparser.add_argument("name", help="configuration name")
    subparser.add_argument("filename", help="destination filename")
    subparser.set_defaults(func=TestbedCDRouter.config_export)

    subparser = subparsers.add_parser("config_check", help="check configuration")
    subparser.add_argument("filename", help="config filename")
    subparser.set_defaults(func=TestbedCDRouter.config_check)

    subparser = subparsers.add_parser("config_import", help="import configuration")
    subparser.add_argument("filename", help="config filename")
    subparser.add_argument("-n", "--name", help="config name, (default: filename)")
    subparser.set_defaults(func=TestbedCDRouter.config_import)

    subparser = subparsers.add_parser(
        "wait_for_netif", help="wait for network interface"
    )
    subparser.add_argument("name", help="interface name")
    subparser.add_argument(
        "-t",
        "--timeout",
        type=int,
        default=60,
        help="wait duration in seconds (default: %(default)s)",
    )
    subparser.set_defaults(func=TestbedCDRouter.wait_for_netif)

    args = parser.parse_args()
    if args.debug:
        logging.getLogger().setLevel(logging.DEBUG)

    if not args.command:
        print("command is missing")
        exit(1)

    cdr = TestbedCDRouter(args)
    args.func(cdr)


if __name__ == "__main__":
    main()
