FROM node:12.16.3-alpine3.9

WORKDIR /buidler-node

RUN apk update
RUN apk upgrade
RUN apk add git python make g++ bash

ADD ./buidler-node /buidler-node 

RUN npm install

# Run an empty command by default - to keep container running
CMD npm run buidler-node