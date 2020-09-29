#!/bin/bash

######################
####### CONFIG #######
######################
RPC_ENDPOINT="http://localhost:8545"
TOTAL_WAITING_TIME=45 # seconds
WAIT_FOR_INPUT=true
ROOT_DIR=$(pwd)
######################

function killCompose {
    cd $ROOT_DIR
    docker-compose down -v
}

function killAndExit {
    echo "####### EXITTING... #######"
    killCompose
    exit 0
}

function graphCreate {
    echo '####### DEPLOYING GRAPH #######'
    cd wildcards-subgraph && yarn codegen && yarn create-local && yarn deploy-local
    if [ "$?" -ne 0 ];
    then
        echo "ERROR: Could not deploy graph successfully - please fix graph errors and press 'g'"
    fi
    cd ..
}

function graphRedeploy {
    echo '####### REDEPLOYING GRAPH #######'
    cd wildcards-subgraph && yarn codegen && yarn deploy-local
    if [ "$?" -ne 0 ];
    then
        echo "ERROR: Could not redeploy graph successfully - please fix graph errors and press 'g'"
    fi
    cd ..
}

function doneLoop {
    echo "######################"
    echo "######## DONE ########"
    if [ $WAIT_FOR_INPUT = true ]; then
        echo "####### PRESS R to RESTART ######"
        echo "# PRESS G to REDEPLOY the graph #"
        echo "######## PRESS Q to QUIT ########"
    fi
    echo "######################"
    if [ $WAIT_FOR_INPUT = true ]; then
        waitForInput
    fi
}

function start {
    echo "####### CLEANUP #######"
    rm -rf data ganache-data
    rm -f contracts/.openzeppelin/dev-321.json

    echo "####### DOCKER-COMPOSE #######"
    docker-compose up 2>&1 > /dev/null &
    DOCKER_COMPOSE_UP_PID=$!

    echo "####### WAITING FOR DOCKERS #######"
    WAITING_TIME=0
    until $(curl --output /dev/null -X POST --silent --fail -H "Content-Type: application/json" --data '{"jsonrpc":"2.0","method":"eth_blockNumber","params":[],"id":1}' $RPC_ENDPOINT); do
        SLEEP_AMOUNT=3
        sleep $SLEEP_AMOUNT
        WAITING_TIME=$(($WAITING_TIME+$SLEEP_AMOUNT))
        if [ "$WAITING_TIME" -gt "$TOTAL_WAITING_TIME" ];
        then
            echo "ERROR: Could not reach ETH chain"
            killAndExit
        fi;
    done

    echo "####### DEPLOYING CONTRACTS #######"
    cd contracts && truffle migrate --reset --network subgraphTest
    if [ "$?" -ne 0 ];
    then
        echo "ERROR: Could not deploy contracts successfully"
        killAndExit
    fi
    cd ..

    graphCreate
    doneLoop
}

function waitForInput {
    while [ true ] ; do
        read -n 1 k <&1
        if [[ $k == "q" || $k == "Q" ]]; then
            echo ""
            killAndExit
        elif [[ $k == "r" || $k == "R" ]]; then
            echo ""
            echo "######## RESTARTING ALL ########"
            killCompose
            sleep 3
            start
        elif [[ $k == "g" || $k == "G" ]]; then
            echo ""
            graphRedeploy
            doneLoop
        fi
    done
}

start