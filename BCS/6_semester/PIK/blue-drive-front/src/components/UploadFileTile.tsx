import React from 'react';
import styled from 'styled-components';
import { fractionToPercentage, bytesToHumanReadable } from '../utils/utils';

const UploadFileStyles = styled.div`
  margin: 1rem;
  display: grid;
  grid-template-columns: auto 1fr;
  grid-template-rows: 1fr auto;
  grid-template-areas:
    'emoji info'
    'bar bar';
  align-items: center;
  border: 1px solid var(--dark);
  border-radius: 1rem;
  background-color: rgba(0, 0, 0, 0.1);
  overflow: hidden;
  font-size: 1.75rem;

  &.success {
    background: linear-gradient(to right, var(--success-dark), var(--success));
  }

  &.failed {
    background: linear-gradient(to right, var(--danger-dark), var(--danger));
  }

  #emoji {
    grid-area: emoji;
    margin: 0 1.5rem;
  }
`;

const UploadFileInfoStyles = styled.div`
  grid-area: info;
  margin-right: 1.5rem;
  display: grid;
  grid-template-rows: repeat(4, auto);

  #filename {
    margin-top: 0.5rem;
  }

  #path {
    font-size: 0.7em;
  }

  #progress {
    margin: 0.5rem 0;
    font-size: 0.7em;
  }
`;

const ProgressBarStyles = styled.div<{ progress: string }>`
  grid-area: bar;
  width: ${props => props.progress};
  height: 4px;
  background: linear-gradient(to right, var(--triadic1), var(--triadic2));
`;

interface UploadFileTileProps {
  file: UploadFile;
}

export default function UploadFileTile({ file }: UploadFileTileProps): JSX.Element {
  const { name, path, status, loaded, total } = file;

  let progress = `${bytesToHumanReadable(loaded)} / ${bytesToHumanReadable(
    total
  )} (${fractionToPercentage(loaded / total)})`;

  if (status === 'success') {
    progress = 'Completed';
  } else if (status === 'failed') {
    progress = 'Failed';
  }

  return (
    <UploadFileStyles className={status}>
      <span id="emoji">📄</span>
      <UploadFileInfoStyles>
        <span id="filename">{name}</span>
        <span id="path">to {path}</span>
        <span id="progress">{progress}</span>
      </UploadFileInfoStyles>
      <ProgressBarStyles progress={fractionToPercentage(loaded / total)} />
    </UploadFileStyles>
  );
}
