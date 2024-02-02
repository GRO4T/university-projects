import React, { useState, useEffect } from 'react';
import UserContext from '../contexts/UserContext';
import axios from 'axios';

function DodajNieobecnosc(props) {

  const { user } = React.useContext(UserContext);

  const [msg, setMsg] = useState("");
  const [msg2, setMsg2] = useState("");

  const [emps, setEmps] = useState([]);
  const [abs, setAbs] = useState([]);
  const [chosenEmp, setChosenEmp] = useState("");

  const [absDate, setAbsDate] = useState({
    from: "",
    to: ""
  });

  useEffect(() => {
    fetch(`/api/employee/by-superior/${user.id}`)
      .then(res => {
        if (res.status === 200) {
          return res.json();
        }
      })
      .then(body => {
        if (!body.length) {
          setMsg("Brak uprawnień lub brak podwładnych.")
        } else {
          setEmps(body.map(p => ({
            id: p.id,
            name: `${p.imie} ${p.nazwisko}`
          })));
        }
      })
      .catch(err => setMsg("Network error"));
  }, [user.id]);

  useEffect(() => {
    setChosenEmp(emps[0]);
  }, [emps])

  useEffect(() => {
    if (chosenEmp?.id || chosenEmp?.id === 0) {
      setMsg("");
      setMsg2("");
      fetchAbs();
    }
    // eslint-disable-next-line react-hooks/exhaustive-deps
  }, [chosenEmp]);

  const fetchAbs = () => {
    axios.get(`/api/absence/by-emp-id/${chosenEmp.id}`)
      .then(res => {
        const conv = res.data.map(a => ({
          from: new Date(a.poczatek.substring(5)),
          to: new Date(a.koniec.substring(5)),
          id: a.id
        }));
        const date = new Date();
        const yest = date.setDate(date.getDate() - 1);
        const filter = conv.filter(c => c.to >= yest);
        filter.sort((a, b) => a.from - b.from);
        setAbs(filter);
      })
      .catch(err => setMsg("Network error"));
  }

  const handleDelete = e => {
    axios.delete(`/api/absence/${e.target.value}`)
      .then(res => fetchAbs());
  }

  const handleSubmit = e => {
    axios.post(`/api/absence`, {
      id: chosenEmp.id,
      koniec: absDate.to,
      poczatek: absDate.from
    })
      .then(res => {
        setMsg2({ ok: "Dodano nieobecność" });
        fetchAbs();
      })
      .catch(err => {
        setMsg2({ bad: "Błędne dane" });
      })
    e.preventDefault();
  }

  return (
    <>
      {msg &&
        <div className="alert alert-danger" role="alert">
          {msg}
        </div>
      }
      {emps.length > 0 &&
        <>
          <div className="form-group row">
            <label htmlFor="pracownik" className="col-sm-2 col-form-label">Pracownik</label>
            <div className="col-sm-4">
              <select id="pracownik" className="form-control" value={chosenEmp?.id}
                onChange={event => setChosenEmp({ id: event.target.value })}>
                {emps.map(d =>
                  <option value={d.id} key={d.id}>{d.name}</option>
                )}
              </select>
            </div>
          </div>
          <hr />
          <table className="table table-hover table-dark" style={{ tableLayout: "fixed" }}>
            <thead>
              <tr>
                <th scope="col">Od</th>
                <th scope="col">Do</th>
                <th scope="col">Akcja</th>
              </tr>
            </thead>
            <tbody>
              {abs.map(a => (
                <tr key={a.from}>
                  <td>{a.from.toLocaleDateString('pl-PL')}</td>
                  <td>{a.to.toLocaleDateString('pl-PL')}</td>
                  <td>
                    <button type="button" className="btn btn-danger btn-sm" value={a.id} onClick={handleDelete}>
                      Usuń
                </button>
                  </td>
                </tr>
              ))}
            </tbody>
          </table>

          <hr />

          <form className="form-inline" onSubmit={handleSubmit}>
            <div className="container">
              <div className="row">
                <div className="col-sm">
                  <div className="form-group mb-2">
                    <label htmlFor="from" className="sr-only">Od</label>
                    <input type="date" className="form-control" id="from" value={absDate.from}
                      onChange={e => setAbsDate(o => ({ ...o, from: e.target.value }))} />
                  </div>
                </div>
                <div className="col-sm">
                  <div className="form-group mx-sm-3 mb-2">
                    <label htmlFor="to" className="sr-only">Do</label>
                    <input type="date" className="form-control" id="to" value={absDate.to}
                      onChange={e => setAbsDate(o => ({ ...o, to: e.target.value }))} />
                  </div>
                </div>
                <div className="col-sm">
                  <button type="submit" className="btn btn-primary mb-2">Dodaj nieobecność</button>
                </div>
              </div>
            </div>
          </form>

          {msg2.bad &&
            <div className="alert alert-danger mt-2" role="alert">
              {msg2.bad}
            </div>
          }
          {msg2.ok &&
            <div className="alert alert-success mt-2" role="alert">
              {msg2.ok}
            </div>
          }
        </>
      }
    </>
  );
}

export default DodajNieobecnosc;