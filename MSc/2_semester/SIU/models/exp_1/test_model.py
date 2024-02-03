import numpy as np
import sys
from keras.models import load_model

import turtlesim_env_single
from dqn_single import DqnSingle


# przykładowe wywołanie uczenia
if __name__ == "__main__":
    env=turtlesim_env_single.provide_env()                      # utworzenie środowiska
    env.setup('routes.csv',agent_cnt=1)                         # połączenie z symulatorem
    env.SPEED_FINE_RATE = -5.0                                  # zmiana wybranych parametrów środowiska
    agents=env.reset()                                          # ustawienie agenta
    tname=list(agents.keys())[0]                                # 'lista agentów' do wytrenowania
    dqns=DqnSingle(env)                                         # utworzenie klasy uczącej
    model_num = sys.argv[1]
    dqns.model=load_model(f"models/dqns2_model_{model_num}.tf")           # albo załadowanie zapisanej wcześniej

    current_state=agents[tname].map
    last_state=[i.copy() for i in current_state]                            # zaczyna od postoju: poprz. stan taki jak obecny
    while True:
        control=np.argmax(dqns.decision(dqns.model,last_state,current_state))
        new_state,reward,done=env.step({tname:dqns.ctl2act(control)})  #krok symulacji
        last_state = current_state
        current_state = new_state
