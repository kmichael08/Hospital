#!/bin/bash
parameterNumber=$#
errorNumbers=0

# kody do polecenia echo do kolorowania wyjscia
GREEN="\033[0;32m"
RED="\033[0;31m"
DEFAULT="\033[0m"

# sprawdzamy liczbe paramerow
if [[ $parameterNumber == 3 ]]
	then flag=$1; program=$2; directory=$3; option="FLAG"
	elif [[ $parameterNumber == 2 ]]
		then program=$1; directory=$2; option="NOFLAG"
	else echo -e "${RED}Wrong number of parameters${DEFAULT}" >&2; exit 1
	fi

# sprawdzamy istnienie katalogu
if [[ ! -d $directory ]]
	then echo -e "${RED}Wrong directory${DEFAULT}" >&2; exit 1
	fi

# sprawdzamy czy istnieja jakiekolwiek pliki wejsciowe
ls $directory/*.in 2> /dev/null > /dev/null
if [[ $? != 0 ]]
	then echo -e "${RED}Empty directory${DEFAULT}" >&2; exit 1
	fi

for f in $directory/*.in; do
echo "$f: ";
if [[ $option == "NOFLAG" ]]
	then
		./$program < $f > temp.out
		# czy program poprawnie sie wykonal
		if [[ $? != 0 ]]
			then echo -e "${RED}program - incorrect${DEFAULT}" >&2; exit 1; fi
		
		diff ${f%in}out temp.out > /dev/null 2>&1
		
		if [[ $? == 0 ]]
			then echo -e "${GREEN}OK ${DEFAULT} - Results correct"
			else echo -e "${RED}ERROR${DEFAULT} - Results incorrect"; ((errorNumbers++))
		fi
		rm temp*
	else
		./$program $flag < $f 2> temp.err > temp.out
		# czy program poprawnie sie wykonal
			if [[ $? != 0 ]]
				then echo -e "${RED}program - incorrect${DEFAULT}" >&2; exit 1; fi

		diff ${f%in}err temp.err > /dev/null 2>&1 
		if [[ $? == 0 ]]
			then echo -e "${GREEN}OK${DEFAULT} - Descriptions correct"
			else echo -e "${RED}ERROR${DEFAULT} - Descriptions incorrect"; ((errorNumbers++));
			fi
		
		diff ${f%in}out temp.out > /dev/null 2>&1
		if [[ $? == 0 ]]
			then echo -e "${GREEN}OK${DEFAULT} - Results correct"
			else echo -e "${RED}ERROR${DEFAULT} - Results incorrect"; ((errorNumbers++)) 
			fi
		rm temp* 
fi
done

# podsumowanie testow
if [[ $errorNumbers == 0 ]]
	then echo -e "${GREEN}WSZYSTKIE TESTY PRZESZLY${DEFAULT}"
	else echo -e "${RED}LICZBA BLEDNYCH TESTOW : $errorNumbers${DEFAULT}" >&2; exit 1
	fi
