#!/bin/bash

set -e

ITEMS_COUNT=100
PORT=55555
IP_ADDRESS="127.0.0.1"

function printList {
	printf "app/list print\n" | nc $IP_ADDRESS $PORT
}

function add {
	for (( i=1; i <= ITEMS_COUNT; i++ ))
	do
		printf "app/list add a$i\n" | nc $IP_ADDRESS $PORT
	done
	printf "\nafter add:\n"
	printList
}

function startAll {
	for (( i=1; i <= ITEMS_COUNT; i++ ))
	do
		printf "app/list/a$i start\n" | nc $IP_ADDRESS $PORT
	done
	printf "\nafter start all items:\n"
	printList
}

function stopAll {
	for (( i=1; i <= ITEMS_COUNT; i++ ))
	do
		printf "app/list/a$i stop\n" | nc $IP_ADDRESS $PORT
	done
	printf "\nafter stop all items:\n"
	printList
}

function delFromMiddle {
	for (( i=0; i <= ITEMS_COUNT; i+=2 ))
	do
		printf "app/list delete a$i\n" | nc $IP_ADDRESS $PORT
	done
	printf "\nafter dell from middle:\n"
	printList
}

function delAllFromFirst {
	for (( i=1; i <= ITEMS_COUNT; i++ ))
	do
		printf "app/list delete a$i\n" | nc $IP_ADDRESS $PORT
	done
	printf "\nafter delete all from first:\n"
	printList
}

function delAllFromLast {
	for (( i=ITEMS_COUNT; i > 0; i-- ))
	do
		printf "app/list delete a$i\n" | nc $IP_ADDRESS $PORT
	done
	printf "\nafter delete all from last:\n"
	printList
}

function delFirst {
	printf "app/list delete a1\n" | nc $IP_ADDRESS $PORT
	printf "\nafter delete first item:\n"
	printList
}

function clearList {
	printf "app/list clear\n" | nc $IP_ADDRESS $PORT
	printf "\nafter clearing list:\n"
	printList
}

function waitItemState {
	while :; do
		sleep 0.5
		var=$(printf "app/list/$1 print\n" | nc $IP_ADDRESS $PORT | tr -d '\0')
		printf "$var\n"
		if echo "$var" | grep $2; then
			break;
		else
			:
		fi
	done
}

function startStopItem {
	local name="testItem"
	echo '------------------------------------------------------------'
	printf "\tcreating new item...\n"
	echo '------------------------------------------------------------'
	printf "app/list add $name\n" | nc $IP_ADDRESS $PORT
	echo '------------------------------------------------------------'
	printf "\tstarting this item...\n"
	echo '------------------------------------------------------------'
	printf "app/list/$name start\n" | nc $IP_ADDRESS $PORT
	echo '------------------------------------------------------------'
	printf "\twaiting this item until RUN state...\n"
	echo '------------------------------------------------------------'
	waitItemState $name "RUN"
	echo '------------------------------------------------------------'
	printf "\tstopping this item...\n"
	echo '------------------------------------------------------------'
	printf "app/list/$name stop\n" | nc $IP_ADDRESS $PORT
	echo '------------------------------------------------------------'
	printf "\twaiting this item until IDLE state...\n"
	echo '------------------------------------------------------------'
	waitItemState $name "IDLE"
	echo '------------------------------------------------------------'
	printf "\tdeleting this item...\n"
	echo '------------------------------------------------------------'
	printf "app/list delete $name\n" | nc $IP_ADDRESS $PORT
}

function overflowTest {
	printf "app/list add dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd\n" | nc $IP_ADDRESS $PORT
}

function appPrint {
	printf "app print\n" | nc $IP_ADDRESS $PORT
}

function appExit {
	printf "app exit\n" | nc $IP_ADDRESS $PORT
}

appPrint

startStopItem

clearList
add
delFromMiddle
clearList

add
delFirst
clearList

add
delAllFromFirst

add
delAllFromLast

add
startAll
stopAll
clearList

overflowTest
overflowTest
overflowTest

appExit
