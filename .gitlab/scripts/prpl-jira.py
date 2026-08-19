#!/usr/bin/env python3

import re
import sys
import os
import glob
import json
import fnmatch
import logging
import argparse

from jira import JIRA
from dataclasses import dataclass


@dataclass
class BuildLogFailure:
    path: str
    step: str
    name: str
    tail_log: list


class OpenWrtSystemInfo:
    def __init__(self, filename):
        with open(filename, "r", errors="ignore") as read_file:
            self.data = json.load(read_file)

    @property
    def kernel(self):
        return self.data["kernel"]

    @property
    def board_name(self):
        n = self.data["board_name"]
        return n.replace(",", "-")

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
        return s.replace("+", "-").replace("/", "-").replace(" ", "_")

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


class RunLogAnalyzer:
    def __init__(self, log_dir=os.getcwd()):
        self._log_dir = log_dir
        self._log_file_patterns = {
            "cram": "cram-result-*.txt",
            "console": "console_*.txt",
            "system_info": "system-*.json",
        }
        self._log_files = {}
        self.analyze_logs()

    def analyze_logs(self):
        for filename in os.listdir(self._log_dir):
            for kind, pattern in self._log_file_patterns.items():
                if fnmatch.fnmatch(filename, pattern):
                    self._log_files[kind] = filename

    def logfile_content(self, kind):
        path = self.logfile_path(kind)
        if not path:
            return

        with open(self.logfile_path(kind), errors="ignore") as f:
            return f.read()

    def logfile_path(self, kind):
        filename = self._log_files.get(kind)
        if not filename:
            return

        return os.path.join(self._log_dir, filename)

    def log_files(self):
        return self._log_files

    def logfile_tail_content(self, kind, count=10):
        path = self.logfile_path(kind)
        if not path:
            return

        bufsize = 8192
        fsize = os.stat(path).st_size

        n = 0
        with open(path, errors="ignore") as f:
            if bufsize > fsize:
                bufsize = fsize - 1

            data = []
            while True:
                n += 1
                f.seek(fsize - bufsize * n)
                data.extend(f.readlines())
                if len(data) >= count or f.tell() == 0:
                    return "".join(data[-count:])

    def system_info(self):
        path = self.logfile_path("system_info")
        if not path:
            return

        return OpenWrtSystemInfo(path)

    def failures_jira_string(self):
        r = ""
        cram_log = self.logfile_content("cram")
        console_log = self.logfile_tail_content("console")
        system_info = self.system_info()

        if system_info:
            r += "Device under test provided following system details:\n"
            r += f" * Board: {system_info.board_name}\n"
            r += f" * Target: {system_info.target}\n"
            r += f" * Version: {system_info.version}\n"
            r += f" * Revision: {system_info.revision}\n"
            r += f" * Kernel: {system_info.kernel}\n"

        if cram_log:
            r += "{code:title=Cram test results}\n"
            r += cram_log
            r += "{code}\n"

        if console_log:
            r += "{code:title=Console log (last 10 lines)}\n"
            r += console_log
            r += "{code}\n"

        return r


class BuildLogAnalyzer:
    # make[3]: *** [Makefile:154: /builds/swpal_6x-uci-1.0.0.1.tar.bz2] Error 128
    RE_MAKE_ERROR = re.compile(r"make\[\d+\]: \*\*\* (.*) Error")

    # Makefile:563: recipe for target 'lldpcli' failed
    RE_MAKE_RECIPE_FAILED = re.compile(
        r"^Makefile:\d+: recipe for target '(.*)' failed"
    )

    def __init__(self, log_dir):
        self.log_dir = log_dir
        self._failures = []
        self.analyze_logs()

    def get_tail_log(self, path):
        tail_log = []
        re_matchers = [self.RE_MAKE_ERROR, self.RE_MAKE_RECIPE_FAILED]

        def add_tail_log(line):
            tail_log_goal = 4
            tail_log.append(line)
            truncate = len(tail_log) - tail_log_goal
            if truncate > 0:
                return tail_log[truncate:]
            return tail_log

        with open(path, "r", errors="ignore") as fd:
            for line in fd:
                line = line.rstrip()
                if len(line) == 0:
                    continue

                tail_log = add_tail_log(line)

                for matcher in re_matchers:
                    if matcher.match(line):
                        return tail_log

    def process_log_file(self, path):
        path = os.path.normpath(path)
        filename = os.path.basename(path)
        dirname = os.path.dirname(path)
        package = dirname.split(os.sep)[-1]
        (step, _) = os.path.splitext(filename)

        if step == "check-compile":
            return

        tail_log = self.get_tail_log(path)
        if not tail_log:
            return

        self._failures.append(BuildLogFailure(path, step, package, tail_log))

    def analyze_logs(self):
        glob_pattern = os.path.join(self.log_dir, "**/**.txt")
        for item in glob.iglob(glob_pattern, recursive=True):
            if os.path.isfile(item):
                self.process_log_file(item)

    def failures(self):
        return self._failures

    def failures_jira_string(self):
        if not self._failures:
            return ""

        r = "\nSeems like there are build issues with following items:\n"
        for f in self._failures:
            r += f" * {f.name} ({f.step}):\n"
            for line in f.tail_log:
                r += f" ** {line}\n"
            r += "\n"

        return r


class JiraHelper:
    def __init__(self, args):
        self.args = args
        self.login()

    def login(self):
        args = self.args
        self.jira = JIRA(
            args.instance_url, basic_auth=(args.api_username, args.api_token)
        )

    def create_or_update_issue(self, failure_type, failure_details):
        args = self.args
        job = os.getenv("CI_JOB_NAME", "job")
        project = os.getenv("CI_PROJECT_NAME", "project")
        project_url = os.getenv("CI_PROJECT_URL", "https://project_url")
        branch = os.getenv("CI_COMMIT_BRANCH", "branch")
        job_url = os.getenv("CI_JOB_URL", "https://job_url")
        commit = os.getenv("CI_COMMIT_SHORT_SHA", "commit_sha")
        commit_message = os.getenv("CI_COMMIT_MESSAGE", "foor bar baz")

        summary = f"CI {failure_type} failure in {project}/{branch} during {job}"

        description = (
            f"Just noticed {failure_type} failure during execution of "
            f"[{job}|{job_url}] CI job in _{project}/{branch}_ which is now at "
            f"[{commit}|{project_url}/-/commit/{commit}] commit:"
            "{noformat}"
            f"{commit_message}"
            "{noformat}"
            f"{failure_details}"
        )

        if args.dry_run:
            logging.info(description)

        jql = f"""
            project={args.project} AND resolution = Unresolved AND summary ~ '{summary}'
        """
        existing_issue = self.jira.search_issues(jql, maxResults=1)
        if existing_issue:
            existing_issue = self.jira.issue(existing_issue[0].key)
            logging.info(
                f"Updating ({existing_issue.key}) {existing_issue.fields.summary}"
            )

            if not args.dry_run:
                self.jira.add_comment(existing_issue, description)

            return existing_issue

        if args.dry_run:
            logging.info(f"Would create new issue: '{self.args.project}/{summary}'")
            return

        new_issue = self.jira.create_issue(
            project=self.args.project,
            summary=summary,
            description=description,
            issuetype={"name": "Bug"},
            fixVersions=[{"name": "1.1"}],
            components=[{"name": "CI"}],
        )

        logging.info(f"Created ({new_issue.key}) {new_issue.fields.summary}")
        return new_issue

    def build_failure(self):
        dry_run = self.args.dry_run
        commit = os.getenv("CI_COMMIT_SHORT_SHA", "commit_sha")
        log_analyzer = BuildLogAnalyzer(self.args.build_logs_dir)
        failures = log_analyzer.failures()

        issue = self.create_or_update_issue(
            "build", log_analyzer.failures_jira_string()
        )

        if not failures:
            return

        if not dry_run and not issue:
            return

        for failure in failures:
            filename = f"{failure.name}_{failure.step}_{commit}.log"

            if dry_run:
                logging.info(f"Would add attachment {filename} from {failure.path})")
                continue

            logging.info(f"Adding attachment {filename} from {failure.path})")
            self.jira.add_attachment(
                issue=issue, attachment=failure.path, filename=filename
            )

    def run_failure(self):
        dry_run = self.args.dry_run
        commit = os.getenv("CI_COMMIT_SHORT_SHA", "commit_sha")
        log_analyzer = RunLogAnalyzer()
        log_files = log_analyzer.log_files()

        issue = self.create_or_update_issue("run", log_analyzer.failures_jira_string())

        if not log_files:
            return

        if not dry_run and not issue:
            return

        for kind, path in log_files.items():
            filename = f"{kind}_{commit}.log"

            if dry_run:
                logging.info(f"Would add attachment {filename} from {path})")
                continue

            logging.info(f"Adding attachment {filename} from {path})")
            self.jira.add_attachment(issue=issue, attachment=path, filename=filename)


def main():
    logging.basicConfig(
        level=logging.INFO, format="%(levelname)7s: %(message)s", stream=sys.stderr
    )

    parser = argparse.ArgumentParser()
    parser.add_argument("-d", "--debug", action="store_true", help="enable debug mode")
    parser.add_argument(
        "--dry-run",
        action="store_true",
        help="Do not alter Jira content, just printout what would be done",
    )

    parser.add_argument(
        "--api-username",
        type=str,
        default=os.environ.get("JIRA_API_USERNAME"),
        help="Jira API username",
    )

    parser.add_argument(
        "--api-token",
        type=str,
        default=os.environ.get("JIRA_API_TOKEN"),
        help="Jira API token",
    )

    parser.add_argument(
        "--instance-url",
        type=str,
        default=os.environ.get("JIRA_INSTANCE_URL", "https://jira.prplfoundation.org"),
        help="Jira instance URL (default: %(default)s)",
    )

    parser.add_argument(
        "--project",
        type=str,
        default=os.environ.get("JIRA_PROJECT", "PCF"),
        help="Jira target project (default: %(default)s)",
    )

    subparsers = parser.add_subparsers(dest="command", title="available subcommands")
    subparser = subparsers.add_parser("build_failure", help="report build failure")
    subparser.add_argument(
        "--build-logs-dir",
        type=str,
        default=os.path.join(os.getcwd(), "logs"),
        help="Path to directory which contains build logs",
    )
    subparser.set_defaults(func=JiraHelper.build_failure)

    subparser = subparsers.add_parser("run_failure", help="report run failure")
    subparser.add_argument(
        "--target",
        type=str,
        help="Target on which runtime testing happend",
    )
    subparser.set_defaults(func=JiraHelper.run_failure)

    args = parser.parse_args()

    if args.debug:
        logging.getLogger().setLevel(logging.DEBUG)

    if not args.command:
        print("command is missing")
        exit(1)

    prpl_jira = JiraHelper(args)
    args.func(prpl_jira)


if __name__ == "__main__":
    main()
