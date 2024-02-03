import React from 'react';
import styled from 'styled-components';
import Nav from './Nav';
import { useAuth } from './AuthContext';

const SidebarStyles = styled.div`
  height: 100%;
  width: 180px;
  padding: 1rem;
  box-sizing: border-box;
  display: grid;
  gap: 1rem;
  grid-template-rows: 1fr auto auto;
  background-color: var(--darker);
  box-shadow: 0 0 16px var(--shadow-color);
  z-index: 1000;
`;

const AuthStyles = styled.div`
  padding: 1rem;
  display: grid;
  gap: 0.7em;
  grid-template-rows: repeat(3, auto);
  justify-items: center;
  background-color: var(--darkest);

  .username {
    color: var(--complementary);
  }

  button {
    width: 100%;
  }
`;

const FooterStyles = styled.footer`
  p {
    margin: 1rem 0;
    text-align: center;
  }
`;

export default function Sidebar(): JSX.Element {
  const authService = useAuth();

  if (authService.authenticated) {
    return (
      <SidebarStyles>
        <Nav />
        <AuthStyles>
          <span>Logged in as</span>
          <span className="username">{authService.username}</span>
          <button type="button" onClick={authService.logout}>
            Log out
          </button>
        </AuthStyles>
        <FooterStyles>
          <p>&copy; BlueDrive {new Date().getFullYear()}</p>
        </FooterStyles>
      </SidebarStyles>
    );
  }

  return <div />;
}
