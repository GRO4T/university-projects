FROM node:15.4.0-alpine3.10 as build
WORKDIR /app
ENV PATH /app/node_modules/.bin:$PATH
RUN npm install react-scripts -g --silent
COPY frontend/package.json ./
COPY frontend/package-lock.json ./
RUN npm ci --silent
COPY ./frontend /app
RUN npm run build

FROM tiangolo/uwsgi-nginx-flask:python3.8
COPY backend/requirements.txt /app
RUN python3 -m pip install -r /app/requirements.txt
COPY backend/app /app
COPY --from=build /app/build /app/static
