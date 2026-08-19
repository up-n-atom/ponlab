# Renaming `pon_pa_template` to a New Name

This module is intended to be a template for creating new PON Adapter (PA) modules.
Therefore, the first task when creating a copy of this code, is to rename it to a New Name.

## To rename all instances of `pon_pa_template` to a new name, follow these steps

1. **Choose the New Name**
	Decide on the new name you want to use (e.g., `new_name`).
	As the name is used in code, the New Name must be a substring, that is valid as part of a C identifier.

2. **Use a Text Editor or IDE**
	Open the project in a text editor or IDE that supports find-and-replace functionality (e.g., VS Code, IntelliJ, Vim).

3. **Perform a Global Search and Replace (Review Each Replacement Carefully)**
	- Search for the term `pon_pa_template`.
	- Replace all occurrences with the new name (e.g., `new_name`).
	- Ensure the search is case-sensitive to avoid unintended replacements.
	- Be aware that other variants of the name (e.g., `libponpatemplate`, or, without prefix, `pa_template`) may exist and could require separate search-and-replace passes or the use of regular expressions to ensure all instances are accurately renamed.

4. **Update File and Folder Names**
	- Rename any files or directories containing `pon_pa_template` in their names to the new name.
	- Use your file manager or a command-line tool like `mv` to rename them.

5. **Verify Changes**
	- Check for any hardcoded references, such as in configuration files or scripts.
	- Run tests or build the project to ensure everything works as expected.

6. **Commit Changes**
	Use version control (e.g., Git) to commit the changes:

	```bash
	git add .
	git commit -m "Renamed pon_pa_template to new_name"
	```

7. **Optional: Automate with a Script**
	If the renaming process is extensive, consider using a script. For example:

	```bash
	find . -type f -exec sed -i 's/pon_pa_template/new_name/g' {} +
	# Note: The following command handles most filenames, but may still fail for some special characters (e.g., newlines).
	find . -depth -name '*pon_pa_template*' -execdir bash -c 'mv -- "$0" "${0//pon_pa_template/new_name}"' {} \;
	```

8. **Test Thoroughly**
	Ensure the application behaves as expected after the renaming.
