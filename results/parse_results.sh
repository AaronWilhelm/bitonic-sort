#/bin/bash
###################################################################################
## This file is part of bitonic-sort
##
## bitonic-sort is free software: you can redistribute it and#or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
##
##  bitonic-sort is distributed in the hope that it will be useful,
##  but WITHOUT ANY WARRANTY; without even the implied warranty of
##  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##  GNU General Public License for more details.
##
##  You should have received a copy of the GNU General Public License
##  along with bitonic-sort.  If not, see <http:##www.gnu.org#licenses#>.
##
##  Copyright 2013 Aaron Wilhelm
###################################################################################
results="results.txt"
touch $results
echo -e "\t\tNumber of processors"
echo -e "Size\t 1\t 2\t 4\t 8\t 16\t 32"
for s in 16 32 64 128
do
    echo -ne "${s}M\t"
    for n_proc in 1 2 4 8 16 32
    do
        nameprefix="RUNp${n_proc}n${s}M"
        files=$(find ./raw -name "${nameprefix}*")
        time_list=( )
        for f in $files
        do
            time_list+="$(awk '/wall/{print substr($5, 0, length($5)-1);}' $f) "
        done
        time=$(echo ${time_list[@]} | tr " " "\n" | awk 'BEGIN{s=0;n=0}
                                                         {s=s+$0;n=n+1}
                                                         END{printf("%2.4f",s/n)}')
	echo -ne "$time\t"
    done
    echo ""
done
