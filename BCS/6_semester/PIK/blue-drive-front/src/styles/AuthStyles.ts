import styled from 'styled-components';

const AuthStyles = styled.div`
  width: 400px;
  margin: 4rem;
  display: grid;
  grid-template-rows: auto 1fr auto;
  grid-auto-rows: auto;
  justify-items: center;

  header {
    font-size: 1.2em;
    font-weight: bolder;
  }

  footer {
    justify-self: start;
    font-size: 1.5rem;
  }
`;

export default AuthStyles;
