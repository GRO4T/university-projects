import React, { useState, useEffect } from 'react';
import axios from 'axios';

function Zastepstwo({ id, poczatek, koniec, name, substitutions, handleUpdate, setMsg }) {

  const [addMode, setAddMode] = useState(false);

  const [drop, setDrop] = useState([]);
  const [form, setForm] = useState("");

  useEffect(() => {
    if (addMode) {
      axios.post(`/api/employee/able-to-substitute`, {
        id_nieobecnosci: id
      })
        .then(res => {
          setForm({ id: res.data[0].id });
          setDrop(res.data);
        })
        .catch(err => 0)
    }
    // eslint-disable-next-line react-hooks/exhaustive-deps
  }, [addMode, id]);

  const handleDelete = () => {
    axios.delete(`/api/substitution/${substitutions[0].id}`)
      .then(res => handleUpdate())
      .catch(err => setMsg("Błąd przy usuwaniu zastępstwa"));
  };

  const handleSubmit = () => {
    axios.post(`/api/substitution`, {
      id_nieobecnosci: id,
      id_pracownika: parseInt(form.id)
    })
      .then(res => {
        setAddMode(false);
        handleUpdate();
      })
      .catch(err => setMsg("Błąd przy dodawaniu zastępstwa"));
  };

  return (
    <>
      <tr>
        <td>{poczatek.toLocaleDateString('pl-PL')}</td>
        <td>{koniec.toLocaleDateString('pl-PL')}</td>
        <td>{name}</td>
        <td>{(!addMode && substitutions?.length > 0) &&
          <>{substitutions[0].imie} {substitutions[0].nazwisko}</>
        }
          {addMode &&
            <select className="form-control form-control-sm" value={form.id} onChange={e => setForm({ id: e.target.value })}>
              {drop.map(d => (
                <option key={d.id} value={d.id}>{d.imie} {d.nazwisko}</option>
              ))}
            </select>
          }</td>
        <td>
          {substitutions?.length > 0
            ? <button type="button" className="btn btn-danger btn-sm" onClick={handleDelete}>Usuń</button>
            : <>{addMode
              ? <>
                <button type="button" className="btn btn-success btn-sm mr-2" onClick={handleSubmit}>Zapisz</button>
                <button type="button" className="btn btn-secondary btn-sm" onClick={() => setAddMode(false)}>Anuluj</button>
              </>
              : <button type="button" className="btn btn-primary btn-sm" onClick={() => setAddMode(true)}>Dodaj</button>
            }</>
          }
        </td>
      </tr>
    </>
  );
}

export default Zastepstwo;