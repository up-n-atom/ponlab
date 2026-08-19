/*
 *  <legal_notice>
 *   MaxLinear, Inc. retains all right, title and interest (including all intellectual
 *   property rights) in and to this computer program, which is protected by applicable
 *   intellectual property laws.  Unless you have obtained a separate written license from
 *   MaxLinear, Inc. or an authorized licensee of MaxLinear, Inc., you are not authorized
 *   to utilize all or a part of this computer program for any purpose (including
 *   reproduction, distribution, modification, and compilation into object code), and you
 *   must immediately destroy or return all copies of this computer program.  If you are
 *   licensed by MaxLinear, Inc. or an authorized licensee of MaxLinear, Inc., your rights
 *   to utilize this computer program are limited by the terms of that license.
 *  
 *   This computer program contains trade secrets owned by MaxLinear, Inc. and, unless
 *   authorized by MaxLinear, Inc. in writing, you agree to maintain the confidentiality
 *   of this computer program and related information and to not disclose this computer
 *   program and related information to any other person or entity.
 *  
 *   Misuse of this computer program or any information contained in it may results in
 *   violations of applicable law.  MaxLinear, Inc. vigorously enforces its copyright,
 *   trade secret, patent, contractual, and other legal rights.
 *  
 *   THIS COMPUTER PROGRAM IS PROVIDED "AS IS" WITHOUT ANY WARRANTIES, AND MAXLINEAR, INC.
 *   EXPRESSLY DISCLAIMS ALL WARRANTIES, EXPRESS OR IMPLIED, INCLUDING THE WARRANTIES OF
 *   MERCHANTIBILITY, FITNESS FOR A PARTICULAR PURPOSE, TITLE, AND NONINFRINGEMENT.
 *  
 *  ***************************************************************************************
 *                                          Copyright (c) 2021/2022, MaxLinear, Inc.
 *  ***************************************************************************************
 *  </legal_notice>
 */

#include "dut/Country.h"

#include "dut/Tools.h"

#include <stdexcept>
#include <vector>

namespace dut {

static const std::vector<CountryInfo> countries = {
    { 0x41, "N/A", "XX", "No Reg Domain" },
    { 234, "UNITED STATES", "US", "USA" },
    { 233, "UNITED KINGDOM", "GB", "Europe" },
    { 112, "JAPAN", "JP", "Japan" },
    { 1, "AFGHANISTAN", "AF", "Europe" },
    { 2, "ALAND ISLANDS", "AX", "Europe" },
    { 3, "ALBANIA", "AL", "Europe" },
    { 4, "ALGERIA", "DZ", "Europe" },
    { 5, "AMERICAN SAMOA", "AS", "Europe" },
    { 6, "ANDORRA", "AD", "Europe" },
    { 7, "ANGOLA", "AO", "Europe" },
    { 8, "ANGUILLA", "AI", "USA" },
    { 9, "ANTARCTICA", "AQ", "Europe" },
    { 10, "ANTIGUA AND BARBUDA", "AG", "USA" },
    { 11, "ARGENTINA", "AR", "USA" },
    { 12, "ARMENIA", "AM", "Europe" },
    { 13, "ARUBA", "AW", "USA" },
    { 14, "AUSTRALIA", "AU", "Japan" },
    { 15, "AUSTRIA", "AT", "Europe" },
    { 16, "AZERBAIJAN", "AZ", "Europe" },
    { 17, "BAHAMAS", "BS", "USA" },
    { 18, "BAHRAIN", "BH", "Europe" },
    { 19, "BANGLADESH", "BD", "Japan" },
    { 20, "BARBADOS", "BB", "USA" },
    { 21, "BELARUS", "BY", "Europe" },
    { 22, "BELGIUM", "BE", "Europe" },
    { 23, "BELIZE", "BZ", "USA" },
    { 24, "BENIN", "BJ", "Europe" },
    { 25, "BERMUDA", "BM", "USA" },
    { 26, "BHUTAN", "BT", "Japan" },
    { 27, "BOLIVIA", "BO", "USA" },
    { 28, "BOSNIA AND HERZEGOVINA", "BA", "Europe" },
    { 29, "BOTSWANA", "BW", "Japan" },
    { 30, "BOUVET ISLAND", "BV", "Japan" },
    { 31, "BRAZIL", "BR", "USA" },
    { 32, "BRITISH INDIAN OCEAN TERRITORY", "IO", "Japan" },
    { 33, "BRUNEI DARUSSALAM", "BN", "Japan" },
    { 34, "BULGARIA", "BG", "Europe" },
    { 35, "BURKINA FASO", "BF", "Japan" },
    { 36, "BURUNDI", "BI", "Europe" },
    { 37, "CAMBODIA", "KH", "Europe" },
    { 38, "CAMEROON", "CM", "Europe" },
    { 39, "CANADA", "CA", "USA" },
    { 40, "CAPE VERDE", "CV", "Japan" },
    { 41, "CAYMAN ISLANDS", "KY", "USA" },
    { 42, "CENTRAL AFRICAN REPUBLIC", "CF", "Europe" },
    { 43, "CHAD", "TD", "Europe" },
    { 44, "CHILE", "CL", "USA" },
    { 45, "CHINA", "CN", "Japan" },
    { 46, "CHRISTMAS ISLAND", "CX", "Japan" },
    { 47, "COCOS (KEELING) ISLANDS", "CC", "Japan" },
    { 48, "COLOMBIA", "CO", "USA" },
    { 49, "COMOROS", "KM", "Europe" },
    { 50, "CONGO", "CG", "Europe" },
    { 51, "CONGO, THE DEMOCRATIC REPUBLIC OF THE", "CD", "Japan" },
    { 52, "COOK ISLANDS", "CK", "Japan" },
    { 53, "COSTA RICA", "CR", "USA" },
    { 54, "COTE D'IVOIRE", "CI", "Japan" }, // arad to do
    { 55, "CROATIA", "HR", "Europe" },
    { 56, "CUBA", "CU", "USA" },
    { 57, "CYPRUS", "CY", "Europe" },
    { 58, "CZECH REPUBLIC", "CZ", "Europe" },
    { 59, "DENMARK", "DK", "Europe" },
    { 60, "DJIBOUTI", "DJ", "Japan" },
    { 61, "DOMINICA", "DM", "USA" },
    { 62, "DOMINICAN REPUBLIC", "DO", "USA" },
    { 63, "ECUADOR", "EC", "USA" },
    { 64, "EGYPT", "EG", "Europe" },
    { 67, "EL SALVADOR", "SV", "USA" },
    { 68, "EQUATORIAL GUINEA", "GQ", "Europe" },
    { 69, "ERITREA", "ER", "Europe" },
    { 70, "ESTONIA", "EE", "Europe" },
    { 71, "ETHIOPIA", "ET", "Europe" },
    { 72, "FALKLAND ISLANDS (MALVINAS)", "FK", "USA" },
    { 73, "FAROE ISLANDS", "FO", "Europe" },
    { 74, "FIJI", "FJ", "Japan" },
    { 75, "FINLAND", "FI", "Europe" },
    { 76, "FRANCE", "FR", "Europe" },
    { 77, "FRENCH GUIANA", "GF", "Europe" },
    { 78, "FRENCH POLYNESIA", "PF", "Europe" },
    { 79, "FRENCH SOUTHERN TERRITORIES", "TF", "Japan" },
    { 80, "GABON", "GA", "Europe" },
    { 81, "GAMBIA", "GM", "Europe" },
    { 82, "GEORGIA", "GE", "Europe" },
    { 83, "GERMANY", "DE", "Europe" },
    { 84, "GHANA", "GH", "Japan" },
    { 85, "GIBRALTAR", "GI", "Europe" },
    { 86, "GREECE", "GR", "Europe" },
    { 87, "GREENLAND", "GL", "USA" },
    { 88, "GRENADA", "GD", "USA" },
    { 89, "GUADELOUPE", "GP", "Europe" },
    { 90, "GUAM", "GU", "Japan" },
    { 91, "GUATEMALA", "GT", "USA" },
    { 92, "GUERNSEY", "GG", "Europe" },
    { 93, "GUINEA", "GN", "Europe" },
    { 94, "GUINEA-BISSAU", "GW", "Europe" },
    { 95, "GUYANA", "GY", "USA" },
    { 96, "HAITI", "HT", "USA" },
    { 97, "HEARD ISLAND AND MCDONALD ISLANDS", "HM", "Japan" },
    { 98, "HOLY SEE (VATICAN CITY STATE)", "VA", "Europe" },
    { 99, "HONDURAS", "HN", "USA" },
    { 100, "HONG KONG", "HK", "Japan" },
    { 101, "HUNGARY", "HU", "Europe" },
    { 102, "ICELAND", "IS", "Europe" },
    { 103, "INDIA", "IN", "Japan" },
    { 104, "INDONESIA", "ID", "Japan" },
    { 105, "IRAN, ISLAMIC REPUBLIC OF", "IR", "Europe" },
    { 106, "IRAQ", "IQ", "Europe" },
    { 107, "IRELAND", "IE", "Europe" },
    { 108, "ISLE OF MAN", "IM", "Europe" },
    { 109, "ISRAEL", "IL", "Europe" },
    { 110, "ITALY", "IT", "Europe" },
    { 111, "JAMAICA", "JM", "USA" },
    { 113, "JERSEY", "JE", "Europe" },
    { 114, "JORDAN", "JO", "Europe" },
    { 115, "KAZAKHSTAN", "KZ", "Europe" },
    { 116, "KENYA", "KE", "Europe" },
    { 117, "KIRIBATI", "KI", "Europe" },
    { 118, "KOREA, DEMOCRATIC PEOPLE'S REPUBLIC OF", "KP", "Japan" },
    { 119, "KOREA, REPUBLIC OF", "KR", "Japan" },
    { 120, "KUWAIT", "KW", "Europe" },
    { 121, "KYRGYZSTAN", "KG", "Europe" },
    { 122, "LAO PEOPLE'S DEMOCRATIC REPUBLIC", "LA", "Japan" },
    { 123, "LATVIA", "LV", "Europe" },
    { 124, "LEBANON", "LB", "Europe" },
    { 125, "LESOTHO", "LS", "Europe" },
    { 126, "LIBERIA", "LR", "Europe" },
    { 127, "LIBYAN ARAB JAMAHIRIYA", "LY", "Europe" },
    { 128, "LIECHTENSTEIN", "LI", "Europe" },
    { 129, "LITHUANIA", "LT", "Europe" },
    { 130, "LUXEMBOURG", "LU", "Europe" },
    { 131, "MACAO", "MO", "Europe" },
    { 132, "MACEDONIA, THE FORMER YUGOSLAV REPUBLIC OF", "MK", "Europe" },
    { 133, "MADAGASCAR", "MG", "Europe" },
    { 134, "MALAWI", "MW", "Europe" },
    { 135, "MALAYSIA", "MY", "Japan" },
    { 136, "MALDIVES", "MV", "Japan" },
    { 137, "MALI", "ML", "Europe" },
    { 138, "MALTA", "MT", "Europe" },
    { 139, "MARSHALL ISLANDS", "MH", "Japan" },
    { 140, "MARTINIQUE", "MQ", "Europe" },
    { 141, "MAURITANIA", "MR", "Europe" },
    { 142, "MAURITIUS", "MU", "Europe" },
    { 143, "MAYOTTE", "YT", "Europe" },
    { 144, "MEXICO", "MX", "USA" },
    { 145, "MICRONESIA, FEDERATED STATES OF", "FM", "Japan" },
    { 146, "MOLDOVA, REPUBLIC OF", "MD", "Europe" },
    { 147, "MONACO", "MC", "Europe" },
    { 148, "MONGOLIA", "MN", "Europe" },
    { 149, "MONTENEGRO", "ME", "Europe" },
    { 150, "MONTSERRAT", "MS", "Europe" },
    { 151, "MOROCCO", "MA", "Europe" },
    { 152, "MOZAMBIQUE", "MZ", "Europe" },
    { 153, "MYANMAR", "MM", "Europe" },
    { 154, "NAMIBIA", "NA", "Europe" },
    { 155, "NAURU", "NR", "Japan" },
    { 156, "NEPAL", "NP", "Japan" },
    { 157, "NETHERLANDS", "NL", "Europe" },
    { 158, "NETHERLANDS ANTILLES", "AN", "USA" },
    { 159, "NEW CALEDONIA", "NC", "Japan" },
    { 160, "NEW ZEALAND", "NZ", "Japan" },
    { 161, "NICARAGUA", "NI", "USA" },
    { 162, "NIGER", "NE", "Europe" },
    { 163, "NIGERIA", "NG", "Europe" },
    { 164, "NIUE", "NU", "Japan" },
    { 165, "NORFOLK ISLAND", "NF", "Japan" },
    { 166, "NORTHERN MARIANA ISLANDS", "MP", "Japan" },
    { 167, "NORWAY", "NO", "Europe" },
    { 168, "OMAN", "OM", "Europe" },
    { 169, "PAKISTAN", "PK", "Europe" },
    { 170, "PALAU", "PW", "Japan" },
    { 171, "PANAMA", "PA", "USA" },
    { 172, "PAPUA NEW GUINEA", "PG", "Europe" },
    { 173, "PARAGUAY", "PY", "USA" },
    { 174, "PERU", "PE", "USA" },
    { 175, "PHILIPPINES", "PH", "Japan" },
    { 176, "PITCAIRN", "PN", "Japan" },
    { 177, "POLAND", "PL", "Europe" },
    { 178, "PORTUGAL", "PT", "Europe" },
    { 179, "PUERTO RICO", "PR", "USA" },
    { 180, "QATAR", "QA", "Europe" },
    { 181, "REUNION", "RE", "Europe" }, // arad todo
    { 182, "ROMANIA", "RO", "Europe" },
    { 183, "RUSSIAN FEDERATION", "RU", "Europe" },
    { 184, "RWANDA", "RW", "Europe" },
    { 185, "SAINT BARTHELEMY", "BL", "USA" }, // arad todo
    { 186, "SAINT HELENA", "SH", "Europe" },
    { 187, "SAINT KITTS AND NEVIS", "KN", "USA" },
    { 188, "SAINT LUCIA", "LC", "USA" },
    { 189, "SAINT MARTIN", "MF", "USA" },
    { 190, "SAINT PIERRE AND MIQUELON", "PM", "USA" },
    { 191, "SAINT VINCENT AND THE GRENADINES", "VC", "USA" },
    { 192, "SAMOA", "WS", "Japan" },
    { 193, "SAN MARINO", "SM", "Europe" },
    { 194, "SAO TOME AND PRINCIPE", "ST", "Europe" },
    { 195, "SAUDI ARABIA", "SA", "Europe" },
    { 196, "SENEGAL", "SN", "Europe" },
    { 197, "SERBIA", "RS", "Europe" },
    { 198, "SEYCHELLES", "SC", "Japan" },
    { 199, "SIERRA LEONE", "SL", "Europe" },
    { 200, "SINGAPORE", "SG", "Japan" },
    { 201, "SLOVAKIA", "SK", "Europe" },
    { 202, "SLOVENIA", "SI", "Europe" },
    { 203, "SOLOMON ISLANDS", "SB", "Japan" },
    { 204, "SOMALIA", "SO", "Europe" },
    { 205, "SOUTH AFRICA", "ZA", "Europe" },
    { 206, "SOUTH GEORGIA AND THE SOUTH SANDWICH ISLANDS", "GS", "Europe" },
    { 207, "SPAIN", "ES", "Europe" },
    { 208, "SRI LANKA", "LK", "Japan" },
    { 209, "SUDAN", "SD", "Europe" },
    { 210, "SURINAME", "SR", "USA" },
    { 211, "SVALBARD AND JAN MAYEN", "SJ", "Europe" },
    { 212, "SWAZILAND", "SZ", "Europe" },
    { 213, "SWEDEN", "SE", "Europe" },
    { 214, "SWITZERLAND", "CH", "Europe" },
    { 215, "SYRIAN ARAB REPUBLIC", "SY", "Europe" },
    { 216, "TAIWAN, PROVINCE OF CHINA", "TW", "Japan" },
    { 217, "TAJIKISTAN", "TJ", "Europe" },
    { 218, "TANZANIA, UNITED REPUBLIC OF", "TZ", "Europe" },
    { 219, "THAILAND", "TH", "Japan" },
    { 220, "TIMOR-LESTE", "TL", "Japan" },
    { 221, "TOGO", "TG", "Europe" },
    { 222, "TOKELAU", "TK", "Japan" },
    { 223, "TONGA", "TO", "Europe" },
    { 224, "TRINIDAD AND TOBAGO", "TT", "Europe" },
    { 225, "TUNISIA", "TN", "Europe" },
    { 226, "TURKEY", "TR", "Europe" },
    { 227, "TURKMENISTAN", "TM", "Europe" },
    { 228, "TURKS AND CAICOS ISLANDS", "TC", "USA" },
    { 229, "TUVALU", "TV", "Japan" },
    { 230, "UGANDA", "UG", "Europe" },
    { 231, "UKRAINE", "UA", "Europe" },
    { 232, "UNITED ARAB EMIRATES", "AE", "Europe" },
    { 235, "UNITED STATES MINOR OUTLYING ISLANDS", "UM", "USA" },
    { 236, "URUGUAY", "UY", "USA" },
    { 237, "UZBEKISTAN", "UZ", "Europe" },
    { 238, "VANUATU", "VU", "Japan" },
    { 240, "VENEZUELA", "VE", "USA" },
    { 241, "VIET NAM", "VN", "Japan" },
    { 242, "VIRGIN ISLANDS, BRITISH", "VG", "USA" },
    { 243, "VIRGIN ISLANDS, U.S.", "VI", "USA" },
    { 244, "WALLIS AND FUTUNA", "WF", "Europe" },
    { 245, "WESTERN SAHARA", "EH", "Europe" },
    { 246, "YEMEN", "YE", "Europe" },
    { 247, "ZAMBIA", "ZM", "Europe" },
    { 248, "ZIMBABWE", "ZW", "Europe" },
};

CountryInfo getCountryByName(const std::string& shortName)
{
    for (const auto& country : countries) {
        if (country.shortName == shortName) {
            return country;
        }
    }

    throw std::invalid_argument("Country '" + shortName + "' not found");
}

CountryInfo getCountryByCode(uint8_t code)
{
    for (const auto& country : countries) {
        if (country.code == code) {
            return country;
        }
    }

    throw std::invalid_argument("Country '" + toString(code) + "' not found");
}

CountryInfo getCountryByIndex(size_t index)
{
    if (index < countries.size()) {
        return countries[index];
    }

    throw std::out_of_range("Index out of range");
}

size_t getCountryCount()
{
    return countries.size();
}

}
