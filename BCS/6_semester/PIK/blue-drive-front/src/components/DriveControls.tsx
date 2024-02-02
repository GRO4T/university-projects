import axios, { AxiosError } from 'axios';
import React, { useState, useRef } from 'react';
import styled from 'styled-components';
import { useToast } from './ToastContext';
import { useUpload } from './UploadContext';
import { isValidDirName } from '../utils/utils';
import { strArrToJoinablePath, strArrToPath } from '../utils/driveUtils';

const DriveControlStyles = styled.div`
  margin: 0 1rem;
  display: flex;
  gap: 2rem;
  align-items: center;

  button {
    min-width: 160px;
  }
`;

const FileInputStyles = styled.input`
  position: absolute;
  width: 0;
  height: 0;
`;

const DirNameInputStyles = styled.input`
  --input-border-color: var(--darkest);

  width: 256px;
  padding: 0.5em;
  background-color: var(--dark);
  border-bottom: 2px solid var(--input-border-color);
`;

interface DriveControlProps {
  path: string[];
}

export default function DriveControl({ path }: DriveControlProps): JSX.Element {
  const [dirNameInput, setDirNameInput] = useState('');
  const [createDirFetchState, setCreateDirFetchState] = useState<FetchState>('idle');

  const fileInput = useRef<HTMLInputElement>(null);

  const launchToast = useToast();
  const { upload, refresh } = useUpload();

  const handleUpload = (e: React.ChangeEvent<HTMLInputElement>) => {
    if (e.target.files) {
      upload(e.target.files, path.length ? strArrToPath(path) : '');
    }
    e.target.value = '';
  };

  const handleCreateDir = () => {
    if (!isValidDirName(dirNameInput)) {
      launchToast('error', 'Invalid name');
      return;
    }
    if (createDirFetchState !== 'pending') {
      setCreateDirFetchState('pending');
      axios
        .post(`/api/directory?path=${strArrToJoinablePath(path)}/${dirNameInput}`)
        .then(() => {
          setCreateDirFetchState('success');
          launchToast('success', `${dirNameInput} created!`);
          setDirNameInput('');
          refresh();
        })
        .catch((err: AxiosError) => {
          setCreateDirFetchState('failed');
          launchToast('error', err.message);
        });
    }
  };

  const isCreateDirDisabled = createDirFetchState === 'pending';

  return (
    <DriveControlStyles>
      <FileInputStyles type="file" multiple onChange={handleUpload} ref={fileInput} />
      <button type="button" className="secondary" onClick={() => fileInput.current?.click()}>
        📤&ensp;Upload Files
      </button>
      <button
        type="button"
        className="secondary"
        onClick={handleCreateDir}
        disabled={isCreateDirDisabled}
      >
        📁&ensp;Create Folder
      </button>
      <DirNameInputStyles
        type="text"
        name="dirName"
        value={dirNameInput}
        onChange={e => setDirNameInput(e.target.value)}
        required
        autoComplete="off"
        placeholder="Folder Name"
        disabled={isCreateDirDisabled}
      />
    </DriveControlStyles>
  );
}
