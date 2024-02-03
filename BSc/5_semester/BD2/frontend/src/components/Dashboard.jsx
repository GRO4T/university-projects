import React, { useState, useEffect } from 'react';
import axios from 'axios';
import UserContext from '../contexts/UserContext';

function Dashboard() {

  const { user } = React.useContext(UserContext);

  const [absFetch, setAbsFetch] = useState([]);
  const [abs, setAbs] = useState([]);

  const [subFetch, setSubFetch] = useState([]);
  const [sub, setSub] = useState([]);

  const [msg, setMsg] = useState("");

  useEffect(() => {
    axios.get(`/api/absence/by-emp-id/${user.id}`)
      .then(res => {
        setAbsFetch(res.data);
      })
      .catch(err => setMsg("Network error"));

    axios.get(`/api/substitution/by-substitute/${user.id}`)
      .then(res => {
        setSubFetch(res.data);
      })
      .catch(err => setMsg("Network error"));
  }, [user.id]);

  useEffect(() => {
    const conv = absFetch.map(a => ({
      from: new Date(a.poczatek.substring(5)),
      to: new Date(a.koniec.substring(5))
    }));
    const date = new Date();
    const yest = date.setDate(date.getDate() - 1);
    const filter = conv.filter(c => c.to >= yest);
    filter.sort((a, b) => a.from - b.from);
    setAbs(filter);
  }, [absFetch]);

  useEffect(() => {
    const conv = subFetch.map(a => ({
      from: new Date(a.poczatek.substring(5)),
      to: new Date(a.koniec.substring(5)),
      for: `${a.imie} ${a.nazwisko}`
    }));
    const date = new Date();
    const yest = date.setDate(date.getDate() - 1);
    const filter = conv.filter(c => c.to >= yest);
    filter.sort((a, b) => a.from - b.from);
    setSub(filter);
  }, [subFetch]);

  return (
    <>
      {msg &&
        <div className="alert alert-danger" role="alert">
          {msg}
        </div>
      }
      <div className="container">
        <div className="row">
          <div className="col-sm">
            <div className="alert alert-primary" role="alert">
              Nadchodzące nieobecności
            </div>
            <table className="table table-hover table-dark">
              <thead>
                <tr>
                  <th scope="col">Od</th>
                  <th scope="col">Do</th>
                </tr>
              </thead>
              <tbody>
                {abs.map(a => (
                  <tr key={a.from}>
                    <td>{a.from.toLocaleDateString('pl-PL')}</td>
                    <td>{a.to.toLocaleDateString('pl-PL')}</td>
                  </tr>
                ))}
              </tbody>
            </table>
          </div>
          <div className="col-sm">
            <div className="alert alert-primary" role="alert">
              Zaplanowane zastępstwa
            </div>
            <table className="table table-hover table-dark">
              <thead>
                <tr>
                  <th scope="col">Od</th>
                  <th scope="col">Do</th>
                  <th scope="col">Za</th>
                </tr>
              </thead>
              <tbody>
                {sub.map(a => (
                  <tr key={a.from}>
                    <td>{a.from.toLocaleDateString('pl-PL')}</td>
                    <td>{a.to.toLocaleDateString('pl-PL')}</td>
                    <td>{a.for}</td>
                  </tr>
                ))}
              </tbody>
            </table>
          </div>
        </div>
      </div>
    </>
  );
}

export default Dashboard;