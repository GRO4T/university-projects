import React from 'react';
import { PageProps } from 'gatsby';
import styled from 'styled-components';
import 'normalize.css';
import GlobalStyles from '../styles/GlobalStyles';
import Typography from '../styles/Typography';
import { ToastContextWrapper } from './ToastContext';
import { UploadContextWrapper } from './UploadContext';
import { AuthContextWrapper } from './AuthContext';
import Sidebar from './Sidebar';

const SiteStyles = styled.div`
  position: relative;
  display: grid;
  grid-template-columns: auto 1fr;
`;

const PageWrapper = styled.div`
  position: relative;
  height: 100vh;
  overflow: auto;
  display: grid;
  align-items: center;
  justify-items: center;
  grid-template-columns: 1fr;
`;

export default function Layout({ children }: PageProps): JSX.Element {
  return (
    <>
      <GlobalStyles />
      <Typography />

      <SiteStyles>
        <ToastContextWrapper>
          <AuthContextWrapper>
            <UploadContextWrapper>
              <Sidebar />

              <PageWrapper>{children}</PageWrapper>
            </UploadContextWrapper>
          </AuthContextWrapper>
        </ToastContextWrapper>
      </SiteStyles>
    </>
  );
}
