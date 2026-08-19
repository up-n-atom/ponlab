#!/bin/sh

export LTQ_CODE_WAVE600="8086:09d0"
export LTQ_CODE_WAVE600D2="8086:0d5a"
export LTQ_CODE_WAVE600D2B="17e6:0d5a"
export LTQ_CODE_WAVE700="1e43:3171"

function get_radio_count() {
        radios_per_wav700=3
        if [ "$OS_NAME" = RDKB ] && [ "$IS_PUMA8" != yes ]; then
                # special case:
                # if we encounter wave700 card on RDKB and it is not Puma 8, assume CGRv3
                # CGRv3 has wave700 card, but 6GHz radio is disabled
                radios_per_wav700=2
        fi
        lspci -n |
                grep -E "$LTQ_CODE_WAVE600|$LTQ_CODE_WAVE600D2|$LTQ_CODE_WAVE600D2B|$LTQ_CODE_WAVE700" |
                awk -v LTQ_CODE_WAVE700="$LTQ_CODE_WAVE700" -v radios_per_wav700="$radios_per_wav700" \
                        'BEGIN{count=0}  {if($0 ~ LTQ_CODE_WAVE700) count += radios_per_wav700; else count += 1;} END{print count}'
}

function get_pci_card_count() {
        lspci -n | grep -c -E "$LTQ_CODE_WAVE600|$LTQ_CODE_WAVE600D2|$LTQ_CODE_WAVE600D2B|$LTQ_CODE_WAVE700"
}

