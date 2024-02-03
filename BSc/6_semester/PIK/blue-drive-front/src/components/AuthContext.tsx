import React, { createContext, useMemo, useContext, useState, useCallback, useEffect } from 'react';
import { throwNoProvider } from '../utils/utils';
import { useToast } from './ToastContext';

interface Credentials {
  authenticated: boolean;
  username: string;
  token: string;
}

const defaultCredentials = {
  authenticated: false,
  username: '',
  token: '',
};

export interface AuthService extends Credentials {
  login: (username: string, token: string) => void;
  logout: () => void;
}

const AuthContext = createContext<AuthService>({
  ...defaultCredentials,
  login: () => throwNoProvider('Auth'),
  logout: () => throwNoProvider('Auth'),
});

export const useAuth = (): AuthService => useContext(AuthContext);

interface AuthContextWrapperProps {
  children: JSX.Element | JSX.Element[];
}

export function AuthContextWrapper({ children }: AuthContextWrapperProps): JSX.Element {
  const [credentials, setCredentials] = useState<Credentials>(defaultCredentials);

  const launchToast = useToast();

  const login = useCallback(
    (username: string, token: string) => {
      setCredentials({
        authenticated: true,
        username,
        token,
      });
      launchToast('success', `Welcome, ${username}!`);
    },
    [launchToast]
  );

  const logout = useCallback(() => {
    setCredentials(defaultCredentials);
    launchToast('info', 'Logout successful!');
  }, [launchToast]);

  useEffect(() => {
    const credStr = sessionStorage.getItem('credentials');
    if (credStr) {
      setCredentials(JSON.parse(credStr));
    }
  }, []);

  useEffect(() => sessionStorage.setItem('credentials', JSON.stringify(credentials)), [
    credentials,
  ]);

  const authService: AuthService = useMemo(
    () => ({
      ...credentials,
      login,
      logout,
    }),
    [credentials, login, logout]
  );

  return (
    <>
      <AuthContext.Provider value={authService}>{children}</AuthContext.Provider>
    </>
  );
}
