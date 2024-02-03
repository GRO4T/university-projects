import jsonlines
import datetime
import time
import requests
import sys
from operator import itemgetter


server = 'http://localhost:8000'
simulation_speed = 500


def handle_event(event, user):
    print(f'{event["session_id"]} of prod {event["product_id"]} at {event["datetime"]} -> {event["event_type"]}')

    if event['first']:
        body = {
            'id': event['session_id'],
            'gender': user['gender'],
            'city': user['city']
        }
        requests.post(f'{server}/api/start_session', json=body)

    json_event = dict(event)
    del json_event['datetime']
    requests.post(f'{server}/api/event', json=json_event)

    if event['last']:
        res = requests.post(f'{server}/api/end_session/{event["session_id"]}')
        total, correct_A, correct_B = itemgetter('total', 'correct_A', 'correct_B')(res.json())

        print(f'    Total predictions:  {total: >3}')
        print(f'    Correct A:          {correct_A: >3}  ->  {round(correct_A / total, 2)}')
        print(f'    Correct B:          {correct_B: >3}  ->  {round(correct_B / total, 2)}')


if __name__ == '__main__':

    if len(sys.argv) > 1:
        simulation_speed = int(sys.argv[1])

    print(f'Simulation speed set to {simulation_speed}x')

    users = {}
    events = []

    with jsonlines.open('simulation-data/users.jsonl') as reader:
        for obj in reader:
            obj['gender'] = 'F' if obj['name'].split()[0][-1] == 'a' else 'M'
            users[obj['user_id']] = obj

    with jsonlines.open('simulation-data/sessions.jsonl') as reader:
        for obj in reader:
            obj['first'] = False
            obj['last'] = False
            obj['datetime'] = datetime.datetime.strptime(obj['timestamp'], '%Y-%m-%dT%H:%M:%S')
            events.append(obj)

    events = sorted(events, key=lambda e: e['timestamp'])

    # mark first event of every session
    visited = set()
    for event in events:
        if event['session_id'] not in visited:
            event['first'] = True
            visited.add(event['session_id'])

    # mark last event of every session
    visited = set()
    for event in reversed(events):
        if event['session_id'] not in visited:
            event['last'] = True
            visited.add(event['session_id'])

    requests.post(f'{server}/api/reset')

    # simulation
    for i in range(len(events)-1):
        event = events[i]
        handle_event(event, users[event['user_id']])
        diff = events[i+1]['datetime'] - event['datetime']
        time.sleep(diff.total_seconds()/simulation_speed)

    last_event = events[len(events)-1]
    handle_event(last_event, users[last_event['user_id']])
