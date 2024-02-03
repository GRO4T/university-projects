from fastapi import FastAPI, WebSocket, WebSocketDisconnect, Request
from fastapi.staticfiles import StaticFiles
from fastapi.templating import Jinja2Templates
from pydantic import BaseModel
from datetime import datetime
import jsonlines
import numpy as np
import pandas as pd

from model_a import ModelA
from model_b import get_model_b


class ConnectionManager:
    def __init__(self):
        self.active_connections = []

    async def connect(self, websocket: WebSocket):
        await websocket.accept()
        self.active_connections.append(websocket)

    async def disconnect(self, websocket: WebSocket):
        self.active_connections.remove(websocket)

    async def broadcast(self, data):
        for connection in self.active_connections:
            try:
                await connection.send_json(data)
            except WebSocketDisconnect:
                await self.disconnect(connection)


class Session(BaseModel):
    id: str
    gender: str
    city: str


class Event(BaseModel):
    session_id: str
    timestamp: str
    product_id: int
    event_type: str
    offered_discount: int
    user_id: int


# Data gathered when the server is running
sessions = []
events = []

predictions = {
    'total': 0,
    'correct_A': 0,
    'correct_B': 0
}


data = pd.read_csv('model-data/data.csv')
model_A = ModelA(data, 20)
model_B = get_model_b(data)


def predict_model_b(session, event_data: Event):
    price_log = data[data.product_id == event_data.product_id].price_log.unique()[0]
    dt_object = datetime.strptime(event_data.timestamp, '%Y-%m-%dT%H:%M:%S')
    model_b_sample = [
        int(event_data.product_id),
        int(session['event_cnt']),
        int(dt_object.hour),
        float(price_log),
        int(event_data.user_id)
    ]
    x = np.array(model_b_sample).reshape((1, -1))
    prediction_B = model_B.predict(x)[0]
    if prediction_B == 1:
        session['prediction_B'] = True
    elif session['prediction_B'] is not True:
        session['prediction_B'] = False


def predict_model_a(session, event_data: Event):
    prediction_A = model_A.predict(event_data.product_id)
    if prediction_A == 1:
        session['prediction_A'] = True
    elif session['prediction_A'] is not True:
        session['prediction_A'] = False


app = FastAPI()
manager = ConnectionManager()

app.mount("/static", StaticFiles(directory="static"), name="static")

templates = Jinja2Templates(directory="templates")


@app.on_event("shutdown")
def shutdown_event():
    global sessions, events
    with jsonlines.open("./output/sessions.jsonl", mode="w") as sessions_file:
        for s in sessions:
            sessions_file.write(s)
    with jsonlines.open("./output/events.jsonl", mode="w") as events_file:
        for e in events:
            events_file.write(e)


@app.get("/")
async def index(request: Request):
    return templates.TemplateResponse("index.html", {"request": request})


@app.get("/api/hello")
async def root():
    # Debug only
    return {"message": "Hello World"}


@app.post("/api/reset")
async def reset():
    global sessions, events, predictions
    sessions = []
    events = []

    predictions = {
        'total': 0,
        'correct_A': 0,
        'correct_B': 0
    }

    await manager.broadcast(sessions)


@app.post("/api/start_session")
async def start_session(session_data: Session):
    session = vars(session_data)
    session['active'] = True
    session['purchased'] = False
    session['prediction_A'] = 'pending'
    session['prediction_B'] = 'pending'
    session['event_cnt'] = 0

    global sessions
    sessions.append(session)

    await manager.broadcast(sessions)

    return session


@app.post("/api/end_session/{session_id}")
async def end_session(session_id):
    global sessions, predictions

    session = [s for s in sessions if s['id'] == session_id][0]
    session['active'] = False

    predictions['total'] += 1
    if session['prediction_A'] is True:
        predictions['correct_A'] += 1
    if session['prediction_B'] is True:
        predictions['correct_B'] += 1

    await manager.broadcast(sessions)

    return predictions


@app.post("/api/event")
async def register_event(event_data: Event):
    global sessions, events

    event = vars(event_data)
    events.append(event)

    session = [s for s in sessions if s['id'] == event['session_id']][0]
    session['event_cnt'] += 1

    predict_model_a(session, event_data)
    predict_model_b(session, event_data)

    if event['event_type'] == 'BUY_PRODUCT':
        session['purchased'] = True

    await manager.broadcast(sessions)

    return session


@app.websocket("/ws/employee_panel")
async def employee_panel(websocket: WebSocket):
    global sessions
    await manager.connect(websocket)
    await manager.broadcast(sessions)
    # otherwise the socket closes immediately after connecting
    while True:
        try:
            await websocket.receive_text()
        except WebSocketDisconnect:
            await manager.disconnect(websocket)
