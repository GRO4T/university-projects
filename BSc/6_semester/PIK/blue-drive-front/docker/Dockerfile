FROM node:14.16.0-buster-slim
WORKDIR /usr/src/BlueDrive/frontend

COPY . .

RUN npm install
RUN npm run build

EXPOSE 8090

CMD ["npm", "run", "serve", "--", "-H", "0.0.0.0", "-p", "8090"]