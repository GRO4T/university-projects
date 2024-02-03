# IUM

Developed and tested on Python 3.8.2

Go to `./backend`.

Run `pip install -r requirements.txt`.

## Server

Run `uvicorn main:app` to start a server.

Go to `http://localhost:8000` to open an employee panel.

You should see `Employee panel - Connected`. When you start a simulation, cards representing sessions will start to appear. Cards with blue background are active sessions. After the session has ended, two vertical bars will appear on the right side of the card. It's a visual representation of whether prediction A (top) and prediction B (bottom) were correct.

To stop the server press `ctrl-c` in the terminal. On server shutdown all the data visible in employee panel will be saved to `./output` directory. `sessions.jsonl` will contain prediction results, which can be later used to evaluate model accuracy.

You can also visit `http://localhost:8000/docs` for a detailed description of provided endpoints. Frontend communicates with backend using WebSockets.

## Simulation

To start a simulation (default speed is `500`):

* in Git Bash for Windows: run `winpty python ./simulation.py [speed]`

* probably everywhere else: run `python ./simulation.py [speed]`

Simulation needs two files to be present in `./simulation-data` directory: `sessions.jsonl` and `users.jsonl`. Those files are already provided, but can be replaced with own data.

If there are two events with timestamps 1 hour apart from each other:

* speed `1` means that the simulation will wait 1 hour before sending the next event

* speed `4` means that the simulation will wait 1/4 hour (or 15 minutes) before sending the next event

The above will not work for very fast speeds since requests are **synchronous**.

**Simulation sends a post request to `/api/reset` at the very beginning, which clears server data. Remember to stop the server after you're done with the simulation if you want to save the prediction data.**
