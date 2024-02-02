import Nav from './components/Nav';
import Index from './components/Index';
import Login from './components/Login';
import Dashboard from './components/Dashboard';
import DodajZastepstwo from './components/DodajZastepstwo';
import DodajNieobecnosc from './components/DodajNieobecnosc';
import EdytujSlownik from './components/EdytujSlownik';
import UserContext from './contexts/UserContext';

import React, { useState, useEffect } from 'react';
import { BrowserRouter as Router, Switch, Route, Redirect } from 'react-router-dom';

function App() {

  const [user, setUser] = useState({
    username: "",
    token: "",
    id: ""
  });

  useEffect(() => {
    const username = sessionStorage.getItem("user");
    const token = sessionStorage.getItem("token");
    if (username && token) {
      setUser({ username, token });
    }
  }, []);

  useEffect(() => {
    if (user.username) {
      fetch(`/api/employee/by-username/${user.username}`)
        .then(res => res.json())
        .then(body => setUser(u => ({ ...u, id: body[0].id })));
    }
  }, [user.username]);

  return (
    <UserContext.Provider value={{ user, setUser }}>
      <Router>
        <Nav />
        {!user.id &&
          <Redirect to="/login" />
        }
        <div className="container">
          <main role="main" className="pb-3">
            <Switch>
              <Route exact path="/">
                {user.id
                  ? <Dashboard />
                  : <Index />
                }
              </Route>
              <Route exact path="/dodajNieobecnosc">
                <DodajNieobecnosc />
              </Route>
              <Route exact path="/dodajZastepstwo">
                <DodajZastepstwo />
              </Route>
              <Route exact path="/edytujSlownik">
                <EdytujSlownik />
              </Route>
              <Route exact path="/login">
                <Login />
              </Route>
            </Switch>
          </main>
        </div>
      </Router>
    </UserContext.Provider>
  );
}

export default App;
