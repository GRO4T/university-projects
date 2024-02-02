import React, { createContext, useCallback, useContext, useMemo, useState } from 'react';
import styled from 'styled-components';
import { getNewUIDGenerator } from '../utils/simpleUID';
import { throwNoProvider } from '../utils/utils';
import Toast from './Toast';

const ToastMountPointStyles = styled.div`
  width: 100%;
  position: absolute;
  grid-column: 2;
  justify-self: center;
  z-index: 1000;
  display: grid;
  justify-items: center;
  pointer-events: none;
`;

export type ToastType = 'error' | 'warning' | 'success' | 'info';

export interface Toast {
  id: number;
  type: ToastType;
  msg: string;
  pop: () => void;
}

type ToastLauncher = (type: ToastType, msg: string) => void;

const ToastContext = createContext<ToastLauncher>(() => throwNoProvider('Toast'));

export const useToast = (): ToastLauncher => useContext(ToastContext);

type ToastContextWrapperProps = {
  children: JSX.Element | JSX.Element[];
};

export function ToastContextWrapper({ children }: ToastContextWrapperProps): JSX.Element {
  const [toasts, setToasts] = useState<Toast[]>([]);

  const generateUID = useMemo(getNewUIDGenerator, []);

  const popFactory = useCallback(
    id => () => setToasts(prevToasts => prevToasts.filter(t => t.id !== id)),
    []
  );

  const launch = useCallback<ToastLauncher>(
    (type, msg) => {
      const id = generateUID();
      const pop = popFactory(id);
      const newToast: Toast = { id, type, msg, pop };
      setToasts(prevToasts => [...prevToasts, newToast]);
      return id + 1;
    },
    [popFactory, generateUID]
  );

  let currentToast = null;
  if (toasts.length) {
    const t = toasts[0];
    currentToast = (
      <Toast key={t.id} type={t.type} msg={t.msg} pop={t.pop} popNow={toasts.length > 1} />
    );
  }

  return (
    <>
      <ToastMountPointStyles>{currentToast}</ToastMountPointStyles>
      <ToastContext.Provider value={launch}>{children}</ToastContext.Provider>
    </>
  );
}
