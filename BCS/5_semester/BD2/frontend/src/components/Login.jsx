import React, { useState, useEffect } from 'react';
import { Redirect } from 'react-router-dom';
import axios from 'axios';
import UserContext from '../contexts/UserContext';

function Login(props) {

  const { user, setUser } = React.useContext(UserContext);

  const [form, setForm] = useState({
    user: "",
    password: ""
  });

  const [msg, setMsg] = useState("");

  useEffect(() => {
    fetch(`/api/test`)
      .then(res => {
        if (res.status !== 200) {
          setMsg("Network error");
        }
      })
      .catch(err => setMsg("Network error"));
  }, []);

  const handleSubmit = e => {
    axios.post(`/api/auth`, {
      username: form.user,
      password: form.password
    })
      .then(res => {
        if (res.status === 200) {
          setUser({
            username: form.user,
            token: res.access_token
          });
          sessionStorage.setItem("user", form.user);
          sessionStorage.setItem("token", res.access_token);
        } else {
          setMsg("Zła nazwa użytkownika lub hasło");
        }
      })
      .catch(err => {
        setMsg("Zła nazwa użytkownika lub hasło");
      });
    e.preventDefault();
  }

  return (
    <>
      {user.id &&
        <Redirect to="/" />
      }
      {msg &&
        <div className="alert alert-danger" role="alert">
          {msg}
        </div>
      }
      <form className="mx-auto col-sm-6" onSubmit={handleSubmit}>
        <div className="form-group">
          <label htmlFor="exampleInputEmail1">Użytkownik</label>
          <input type="text" className="form-control" id="exampleInputEmail1" aria-describedby="emailHelp" value={form.user} onChange={e => setForm(o => ({ ...o, user: e.target.value }))} />
        </div>
        <div className="form-group">
          <label htmlFor="exampleInputPassword1">Hasło</label>
          <input type="password" className="form-control" id="exampleInputPassword1" value={form.password} onChange={e => setForm(o => ({ ...o, password: e.target.value }))} />
        </div>
        <button type="submit" className="btn btn-primary">Zaloguj</button>
      </form>
    </>
  );
}

export default Login;