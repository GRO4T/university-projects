import React from 'react';
import styled from 'styled-components';
import { Link } from 'gatsby';
import HelloWorld from '../components/HelloWorld';
import ToastLauncher from '../components/ToastLauncher';
import { useAuth } from '../components/AuthContext';

const WrapperStyles = styled.div`
  padding: 2rem;
  display: grid;
  grid-template-rows: auto auto;
  justify-items: center;
`;

export default function HomePage(): JSX.Element {
  const authService = useAuth();

  return (
    <WrapperStyles>
      <HelloWorld />
      <ToastLauncher />
      {authService.authenticated || <Link to="/app/login">Login screen ➡️</Link>}
    </WrapperStyles>
  );
}
