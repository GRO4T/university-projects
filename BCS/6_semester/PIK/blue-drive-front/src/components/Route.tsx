import React, { FunctionComponent } from 'react';
import { navigate } from 'gatsby';
import { RouteComponentProps } from '@reach/router';
import { useAuth } from './AuthContext';

interface RouteProps extends RouteComponentProps {
  component: FunctionComponent;
  restricted?: boolean;
  auth?: boolean;
}

export default function Route({
  component: Component,
  restricted,
  auth,
}: RouteProps): JSX.Element | null {
  const authService = useAuth();

  if (auth && authService.authenticated) {
    void navigate('/app/drive');
    return null;
  }

  if (restricted && !authService.authenticated) {
    void navigate('/app/login');
    return null;
  }

  return <Component />;
}
