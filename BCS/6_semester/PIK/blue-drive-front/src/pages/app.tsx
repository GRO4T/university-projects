import React from 'react';
import { Router } from '@reach/router';
import DrivePage from '../app/drive';
import LoginPage from '../app/login';
import RegisterPage from '../app/register';
import Route from '../components/Route';

export default function App(): JSX.Element {
  return (
    // https://github.com/reach/router/issues/63#issuecomment-457840121
    <Router component={({ children }) => <>{children}</>}>
      <Route restricted path="/app/drive" component={DrivePage} />
      <Route auth path="/app/login" component={LoginPage} />
      <Route auth path="/app/register" component={RegisterPage} />
    </Router>
  );
}
