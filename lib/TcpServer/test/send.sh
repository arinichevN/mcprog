#!/bin/bash

printf "app print" | nc 127.0.0.1 55555
printf "app/list print" | nc 127.0.0.1 55555
printf "app/list add object1" | nc 127.0.0.1 55555
printf "app/list/object1 print" | nc 127.0.0.1 55555
printf "app/list add object2" | nc 127.0.0.1 55555
printf "app/list add object3" | nc 127.0.0.1 55555
printf "app/list add object4" | nc 127.0.0.1 55555
printf "app/list clear" | nc 127.0.0.1 55555
printf "app exit" | nc 127.0.0.1 55555
