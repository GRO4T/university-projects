import React from 'react';
import styled, { keyframes } from 'styled-components';

const spinnerKeyframes = keyframes`
  0%{
    border-color: var(--triadic2);
    transform: scale(0.8);
  }

  100%{
    border-color: var(--triadic1);
    transform: scale(1.2);
  }
`;

const SpinnerStyles = styled.div`
  --size: 48px;
  width: var(--size);
  height: var(--size);
  border: 6px solid var(--primary);
  border-radius: 100%;
  animation: ${spinnerKeyframes} 1.5s cubic-bezier(0.3, 0, 0.7, 1) infinite alternate;
`;

export default function Spinner(): JSX.Element {
  return <SpinnerStyles />;
}
