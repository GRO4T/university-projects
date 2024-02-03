import React from 'react';
import styled from 'styled-components';
import Spinner from './Spinner';

const WrapperStyles = styled.div`
  position: relative;
  width: 100%;
  height: 100%;
`;

const LoadingStyles = styled.div`
  position: absolute;
  width: 100%;
  height: 100%;
  display: grid;
  align-items: center;
  justify-content: center;
  z-index: 100;
`;

const BlurStyles = styled.div`
  width: 100%;
  height: 100%;
  filter: blur(4px);
`;

export interface LoadingBlurProps {
  fetchState: FetchState;
  children: JSX.Element | JSX.Element[];
}

export default function LoadingBlur({ fetchState, children }: LoadingBlurProps): JSX.Element {
  if (fetchState === 'pending') {
    return (
      <WrapperStyles>
        <LoadingStyles>
          <Spinner />
        </LoadingStyles>
        <BlurStyles>{children}</BlurStyles>
      </WrapperStyles>
    );
  }

  return <>{children}</>;
}
