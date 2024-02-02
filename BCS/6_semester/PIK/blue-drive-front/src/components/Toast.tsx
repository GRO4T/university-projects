import React, { useRef, useEffect } from 'react';
import styled, { keyframes } from 'styled-components';
import { ToastType } from './ToastContext';

const toastSlideIn = keyframes`
  0%{
    opacity: 0;
    transform: translateY(-200%);
  }
`;

const ToastStyles = styled.div<{ slideInDuration: string; hidingDuration: string }>`
  --bg-color: var(--info);
  --shadow-color: var(--info-dark);

  min-width: 400px;
  max-width: 800px;
  /* width: clamp(250px, 60%, 800px); */
  position: absolute;
  padding: 1rem;
  background-color: var(--bg-color);
  border-radius: 0.5rem;
  box-shadow: 0 0 4px 2px var(--shadow-color);
  box-sizing: border-box;
  filter: contrast(0.8);
  pointer-events: auto;
  cursor: default;
  animation: ${toastSlideIn} ${props => props.slideInDuration} cubic-bezier(0, 1, 0.7, 1.3);
  transform: translateY(100%);
  transition: opacity ${props => props.hidingDuration};

  &.error {
    --bg-color: var(--danger);
    --shadow-color: var(--danger-dark);
  }

  &.warning {
    --bg-color: var(--warning);
    --shadow-color: var(--warning-dark);
  }

  &.success {
    --bg-color: var(--success);
    --shadow-color: var(--success-dark);
  }

  &.hiding {
    opacity: 0;
  }
`;

export interface ToastProps {
  type: ToastType;
  msg: string;
  pop: () => void;
  popNow: boolean;
}

const SLIDE_IN_DURATION = 300;
const HIDING_DURATION = 1000;

const DEFAULT_TIMEOUT = 4000;
const SHORT_TIMEOUT = SLIDE_IN_DURATION + 200;

export default function Toast({ type, msg, pop, popNow }: ToastProps): JSX.Element {
  const toast = useRef<HTMLDivElement>(null);

  const cssSlideInDuration = `${SLIDE_IN_DURATION / 1000}s`;
  const cssHidingDuration = `${HIDING_DURATION / 1000}s`;

  useEffect(() => {
    const timeout = setTimeout(
      () => {
        toast.current?.classList.add('hiding');
        setTimeout(pop, HIDING_DURATION);
      },
      popNow ? SHORT_TIMEOUT : DEFAULT_TIMEOUT
    );

    return () => clearTimeout(timeout);
  }, [pop, popNow]);

  return (
    <ToastStyles
      slideInDuration={cssSlideInDuration}
      hidingDuration={cssHidingDuration}
      className={type}
      ref={toast}
      onClick={pop}
    >
      {msg}
    </ToastStyles>
  );
}
