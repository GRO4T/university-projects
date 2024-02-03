import { useEffect, useState } from 'react';
import styled from 'styled-components';
import { Tile } from './Tile';

const WrapperStyles = styled.div`
  width: 100%;
  height: 100%;
  display: grid;
  grid-template-rows: auto 1fr;
  overflow: auto;
`;

const HeaderStyles = styled.header`
  margin-top: 1rem;
  display: flex;
  align-items: center;
  justify-content: center;
  font-size: 1.75rem;
`;

const MainStyles = styled.main`
  height: fit-content;
  padding: 1rem;
  display: grid;
  gap: 1rem;
  grid-template-columns: repeat(auto-fill, minmax(360px, 1fr));
  align-items: center;
`;

const isDev = process.env.NODE_ENV === 'development';

function App() {
  const [connStatus, setConnStatus] = useState('Connecting...');
  const [sessions, setSessions] = useState([]);

  useEffect(() => {
    const ws = new WebSocket(
      `${isDev ? 'ws://localhost:8000' : `ws://${window.location.hostname}:8000`}/ws/employee_panel`
    );

    ws.onopen = () => setConnStatus('Connected');
    ws.onclose = () => setConnStatus('Disconnected');
    ws.onerror = () => setConnStatus('Error!');

    ws.onmessage = e => setSessions(JSON.parse(e.data));
  }, []);

  return (
    <WrapperStyles>
      <HeaderStyles>Employee panel - {connStatus}</HeaderStyles>
      <MainStyles>
        {sessions.map(d => (
          <Tile key={d.id} data={d} />
        ))}
      </MainStyles>
    </WrapperStyles>
  );
}

export default App;
