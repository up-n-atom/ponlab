#!/bin/sh

UCI_FUNCTIONS=/lib/functions.sh
JSON_FUNCTIONS=/usr/share/libubox/jshn.sh

DSL_UCI_CONFIG="dsl"
DSL_UCI_SECTION_TYPE="entry"
DSL_CONFIG=/tmp/dsl/dsl.conf
DSL_CONFIG_OBJECT="dsl"
DSL_DBG_OBJECT="debugging"
DSL_EXTRA_OBJECT="extra"

DSL_SCRIPT_NAME="dsl_uci2json.sh"
DSL_DEBUG=0
# Max number of elements to operate on JSON file parameter access (incl. leaf)
MAX_LOOP_COUNT=5


if [ ! -f ${UCI_FUNCTIONS} ]; then
	echo "${DSL_SCRIPT_NAME}: Missing file \"${UCI_FUNCTIONS}\""
	return
fi

if [ ! -f ${JSON_FUNCTIONS} ]; then
	echo "${DSL_SCRIPT_NAME}: Missing file \"${JSON_FUNCTIONS}\""
	return
fi

. ${UCI_FUNCTIONS}
. ${JSON_FUNCTIONS}

# Logging function
#
# input:
#       string
# effect:
#       prints fixed prefix and passed string onto standard output
# return:
#       none
dsl_debug_print()
{
	if [ ! -z ${DSL_DEBUG} ] && [ ${DSL_DEBUG} -eq 1 ]; then
		echo "${DSL_SCRIPT_NAME}: $1" > /dev/console
	fi
}

# Floating point number test function
#
# intput:
#       uci value
# effect:
#       checks format of the value
# return:
#       1 if value is a float
#       0 if value is not an float
dsl_is_float()
{
	local value=$1
	local result

	result=$(echo -n ${value} | grep -E '^[-+]?[0-9]+[.][0-9]+$')

	if [ ! -z ${result} ]; then
		echo "1"
	else
		echo "0"
	fi
}

# Integer test function
#
# intput:
#       uci value
# effect:
#       checks format of the value
# return:
#       1 if value is an integer
#       0 if value is not an integer
dsl_is_int()
{
	local value=$1
	local result

	result=$(echo -n ${value} | grep -E '^[-+]?[0-9]+$')

	if [ ! -z ${result} ]; then
		echo "1"
	else
		echo "0"
	fi
}

# UCI callback function triggered by UCI config_load on each config keyword
# found in uci config file
#
# input:
#       config type
# effect:
#       check type and override option_cb to handle each option and value in
#       that config entry
# return:
#       none
config_cb()
{
	local type="$1"

	if [ ! -z $type ] && [ ${type} == "${DSL_UCI_SECTION_TYPE}" ]; then
		# handle UCI option
		option_cb()
		{
			local option="$1"
			local value="$2"

			eval dsl_json_write "option" ${CONFIG_SECTION} \"${option}\" \"${value}\"
		}
		list_cb()
		{
			local list="$1"
			local value="$2"

			eval dsl_json_write "list" ${CONFIG_SECTION} \"${list}\" \"${value}\"
		}
	else
		option_cb() { return; }
		list_cb() { return; }
	fi
}

# Stores uci option and value into json.
# If corresponding json object does not exist - creates it.
#
# input:
#       uci entry
#       dedicated json write function (for: int, float, string)
#       uci option
#       uci value
# effect:
#       selects/adds json object and stores pair (option, value) into json
# return:
#       none
dsl_json_update_entry()
{
	local object=$1
	local cb_function=$2
	shift 2
	local cb_args=$@
	local no_object=0

	dsl_debug_print "dsl_json_update_entry(): '${object}', '${cb_function}',  '${cb_args}'."

	json_select ${object} || no_object=1

	if [ ${no_object} -eq 1 ]; then
		dsl_debug_print "Adding missing object '${object}'..."
		json_add_object ${object}
	fi

	eval ${cb_function} ${cb_args}

	if [ ${no_object} -eq 1 ]; then
		json_close_object ${object}
	else
		json_select ..
	fi
}

# Stores uci array and value into json.
# If corresponding json array does not exist - creates it.
#
# input:
#       uci entry
#       dedicated json write function (for: int, float, string)
#       uci option
#       uci value
# effect:
#       selects/adds json object and stores pair (option, value) into json
# return:
#       none
dsl_json_append_to_array()
{
	local object=$1
	local cb_function=$2
	local array=$3
	shift 2
	local cb_args=$@
	local no_object=0
	local no_array=0

	dsl_debug_print "dsl_json_append_to_array(): '${object}', '${cb_function}', '${array}', '${cb_args}'."

	# WORKAROUND (start)
	# Entry with name same as its list name
	# will be stored in JSON just as array of that name
	# (not as object and that array inside it).
	# e.g. (case with workaround):
	# input:
	# config entry 'A'
	#     list A '0x1'
	#     list A '0x2'
	# output:
	# "A": [ "0x1", "0x2" ]
	if [ ${object} != ${array} ]; then
		json_select ${object} || no_object=1

		if [ ${no_object} -eq 1 ]; then
			dsl_debug_print "Adding missing object '${object}'..."
			json_add_object ${object}
		fi
	fi

	json_select ${array} || no_array=1

	if [ ${no_array} -eq 1 ]; then
		dsl_debug_print "Adding array '${array}'..."
		json_add_array ${array}
	fi

	eval ${cb_function} ${cb_args}

	if [ ${no_array} -eq 1 ]; then
		json_close_array ${array}
	else
		json_select ..
	fi

	# WORKAROUND (finish)
	if [ ${object} != ${array} ]; then
		if [ ${no_object} -eq 1 ]; then
			json_close_object ${object}
		else
			json_select ..
		fi
	fi
}

# Generic function to write specific types into daemon's config file
#
# input:
#       uci entry type
#       uci entry name
#       uci option
#       uci value
# effect:
#       calls final-update function
# return:
#       none
dsl_json_write()
{
	local type=$1
	local entry=$2
	local element=$3
	local value=$4
	local is_float=0
	local is_int=0
	local func_json_add_function=json_add_string
	local func_json_update_entry=dsl_json_update_entry

	dsl_debug_print "dsl_json_write(): Writing JSON '$type $entry->$element:$value'..."

	is_float=$(dsl_is_float $value)
	is_int=$(dsl_is_int $value)

	if [ ${is_float} -eq 1 ]; then
		func_json_add_function=json_add_double
	elif [ ${is_int} -eq 1 ]; then
		func_json_add_function=json_add_int
	fi

	if [ ! -z $type ] && [ ${type} == "list" ]; then
		func_json_update_entry=dsl_json_append_to_array
	fi

	eval ${func_json_update_entry} ${entry} ${func_json_add_function} \"${element}\" \"${value}\"
}

# Updates additional time and date field on each config translation
#
# input:
#       none
# effect:
#       Latest time and date of the translation
# return:
#       none
dsl_update_time()
{
	local option="Last update time"
	local value=$(date +"%F %T")

	dsl_json_update_entry ${DSL_EXTRA_OBJECT} json_add_string \"${option}\" \"${value}\"
}

# Make config file human readable again
#
# input:
#	none
# effect:
#	Config file is not one-liner anymore.
#	Adds some new lines (and tabs) here and there.
# return:
#	none
dsl_config_pretty()
{
	sed -i 's/},/\n\t},\n/g' ${DSL_CONFIG}
	sed -i 's/: {/: {\n\t/g' ${DSL_CONFIG}
	sed -i -E 's/, \"([a-z]+)/,\n\t\"\1/g' ${DSL_CONFIG}
}

# Get value of given property (object in json file)
#
# Notes:
# 1) To see echo related printouts on the console "set -x" command shall be used
# 2) In case of enabling echo prints which are directed to the console within
#    this function will result in failure of evaluating the return value because
#    all printed strings will be used as return value!
#    This might be helpfull for debugging this function itself but further
#    processing the return value will not work.
#
# input:
#       Property path as string
#       Property name as string
# effect:
#       Searched json config for given property
# return:
#       Value of given property or an empty string in case of an error
dsl_get_cfg_value()
{
	local prop_path=$1
	local prop_name=$2
	local no_object=0
	local prop_value=""
	# Start search with any non-empty string
	local object_name="."

	json_load_file ${DSL_CONFIG_JSON}

	nLoopCnt=0
	while [ ! -z "$object_name" -a $nLoopCnt -le $MAX_LOOP_COUNT ]; do
		nLoopCnt=$(($nLoopCnt+1))
		object_name="$(echo ${prop_path} | cut -d . -f${nLoopCnt})"
		#echo "object_name=${object_name}"
		if [ ! -z $object_name ]; then
			json_select ${object_name} || no_object=1
			if [ ${no_object} -ne 0 ]; then
				return
			fi
		fi
	done

	if [ $nLoopCnt -le $MAX_LOOP_COUNT ]; then
		echo ${prop_name} | grep -e "\." -q
		# Check if property contains dot (.)  delimiter
		# and treat first substring as object
		# and second as final property
		if [ $? = 0 ]; then
			local subobject="$(echo "${prop_name}" | cut -d. -f1)"
			json_select ${subobject} || no_object=1
			if [ ${no_object} -eq 0 ]; then
				prop_name="$(echo "${prop_name}" | cut -d. -f2)"
				json_get_var prop_value ${prop_name}
				json_select ..
			fi
			json_select ..
		else
			json_get_var prop_value ${prop_name}
			json_select ..
		fi
	fi

	#echo "${prop_path}.${prop_name}='${prop_value}'"
	echo ${prop_value}
}

# Gets value of given property (Debugging object in json config)
#
# input:
#       Property name as string
# effect:
#	Searched json config for given property
# return:
#       Value of given property or null
dsl_get_dbg_value()
{
	local prop_name=$1
	local no_object=0
	local prop_value=""

	if [ ! -f ${DSL_CONFIG} ]; then
		return
	fi

	json_load_file ${DSL_CONFIG}
	json_select ${DSL_CONFIG_OBJECT} > /dev/null || no_object=1
	if [ ${no_object} -eq 0 ]; then
		json_select ${DSL_DBG_OBJECT} > /dev/null || no_object=1
		if [ ${no_object} -eq 0 ]; then
			echo ${prop_name} | grep -e "\." -q
			# Check if property contains dot (.)  delimiter
			# and treat first substring as object
			# and second as final property
			if [ $? = 0 ]; then
				local subobject="$(echo "${prop_name}" | cut -d. -f1)"
				json_select ${subobject} > /dev/null || no_object=1
				if [ ${no_object} -eq 0 ]; then
					prop_name="$(echo "${prop_name}" | cut -d. -f2)"
					json_get_var prop_value ${prop_name}
					json_select ..
				fi
				json_select ..
			else
				json_get_var prop_value ${prop_name}
				json_select ..
			fi
		fi
		json_select ..
	fi

	echo ${prop_value}
}

# Sets value of given property (startup_init object in json config)
#
# input:
#	Property name as string
#	Property value as string
# effect:
#       Updated json config for given property
# return:
#       none
dsl_set_startup_value()
{
	local DSL_STARTUP_OBJECT="startup_init"
	local prop_name=$1
	local prop_value=$2
	local no_object=0

	if [ -f ${DSL_CONFIG} ]; then
		json_init
		json_load_file ${DSL_CONFIG}

		json_select ${DSL_CONFIG_OBJECT} || no_object=1
		if [ ${no_object} -eq 0 ]; then
			json_select ${DSL_STARTUP_OBJECT} || no_object=1
			if [ ${no_object} -eq 0 ]; then
				if [ ! -z ${prop_name+x} ] && [ ! -z ${prop_value+x} ]; then
					json_add_int ${prop_name} ${prop_value}
				fi
				json_select ..
			fi
			json_select ..
		fi

		dsl_update_time
		echo "$(json_dump)" > ${DSL_CONFIG}
	fi
}

# Main function translating uci config file into daemon's config file (json
# format)
#
# input:
#       none
# effect:
#       DSL_CONFIG file with proper json formatted content
# return:
#       none
dsl_config_translate()
{
	local use_webui_dsl_conf=1
	local no_object=0

	dsl_debug_print "dsl_config_translate(): Translating..."

	json_init

	if [ -f ${DSL_CONFIG} ]; then
		json_load_file ${DSL_CONFIG}
		json_select ${DSL_EXTRA_OBJECT} || no_object=1
		if [ ${no_object} -eq 0 ]; then
			json_get_var use_webui_dsl_conf use_web_ui_dsl_conf
			json_select ..
		fi

		if [ -z ${use_webui_dsl_conf} ] || [ ${use_webui_dsl_conf} -eq 1 ]; then
			# WORKAROUND: the sed function is currently used to clean the XTSE
			# array before loading the configuration file
			sed -i 's/"xtse": \[.*\]/"xtse": \[\]/' ${DSL_CONFIG}
			json_cleanup
			json_load_file ${DSL_CONFIG}
		fi
	fi

	if [ -z ${use_webui_dsl_conf} ] || [ ${use_webui_dsl_conf} -eq 1 ]; then
		dsl_json_update_entry ${DSL_CONFIG_OBJECT} config_load ${DSL_UCI_CONFIG}
	fi

	dsl_update_time

	echo "$(json_dump)" > ${DSL_CONFIG}
	dsl_config_pretty

	# security reason - set dslgrp group owner
	chown :dslgrp ${DSL_CONFIG}
	chmod u=rw,g=r,o= ${DSL_CONFIG}

	dsl_debug_print "dsl_config_translate(): Translated"
}
