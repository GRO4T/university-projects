import React, { useState, useEffect } from 'react';
import UserContext from '../contexts/UserContext';
import axios from 'axios';
import Zastepstwo from './Zastepstwo';

function DodajZastepstwo(props) {

  const { user } = React.useContext(UserContext);

  const [msg, setMsg] = useState("");
  const [abs, setAbs] = useState([]);

  useEffect(() => {
    fetchAbs();
    // eslint-disable-next-line react-hooks/exhaustive-deps
  }, []);

  const fetchAbs = () => {
    axios.get(`/api/employee/${user.id}/subordinate-abs-subs`)
      .then(res => {
        if (!res.data.length) {
          setMsg("Brak uprawnień lub brak nieobecności.");
        } else {
          const absf = res.data.reduce((acc, cur) => {
            const name = `${cur.imie} ${cur.nazwisko}`;
            const abse = cur.absences.map(a => ({
              ...a,
              name,
              koniec: new Date(a.koniec.substring(5)),
              poczatek: new Date(a.poczatek.substring(5))
            }));
            return [...acc, ...abse];
          }, []);
          const date = new Date();
          const yest = date.setDate(date.getDate() - 1);
          const filter = absf.filter(a => a.koniec >= yest);
          filter.sort((a, b) => a.poczatek - b.poczatek);
          setAbs(filter);
          if(filter.length === 0){
            setMsg("Brak uprawnień lub brak nieobecności.");
          }
        }
      })
      .catch(err => setMsg('Network error'));
  };

  return (
    <>
      {msg &&
        <div className="alert alert-danger" role="alert">
          {msg}
        </div>
      }
      {abs.length > 0 &&
        <>
          <table className="table table-hover table-dark" style={{ tableLayout: "fixed" }}>
            <thead>
              <tr>
                <th scope="col">Od</th>
                <th scope="col">Do</th>
                <th scope="col">Za</th>
                <th scope="col">Kto</th>
                <th scope="col">Akcja</th>
              </tr>
            </thead>
            <tbody>
              {abs.map(a => (
                <Zastepstwo key={a.id} id={a.id} poczatek={a.poczatek} koniec={a.koniec} name={a.name} substitutions={a.substitutions} handleUpdate={fetchAbs} setMsg={setMsg} />
              ))}
            </tbody>
          </table>
        </>
      }
    </>
  );
}

export default DodajZastepstwo;