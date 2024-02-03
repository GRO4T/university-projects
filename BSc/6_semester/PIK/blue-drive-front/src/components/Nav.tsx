import React from 'react';
import { Link } from 'gatsby';
import styled from 'styled-components';

const NavStyles = styled.nav`
  ul {
    margin: 0;
    padding: 0;
    text-align: center;
    list-style: none;
    display: grid;
    grid-auto-rows: auto;
  }

  li {
    display: block;
  }

  a {
    display: block;
    width: 100%;
    padding: 1rem 0;
    color: var(--primary);

    &:hover,
    &[aria-current] {
      background-color: var(--darkest);
    }
  }
`;

export default function Nav(): JSX.Element {
  return (
    <NavStyles>
      <ul>
        <li>
          <Link to="/">Home</Link>
        </li>
        <li>
          <Link to="/app/drive">Drive</Link>
        </li>
      </ul>
    </NavStyles>
  );
}
