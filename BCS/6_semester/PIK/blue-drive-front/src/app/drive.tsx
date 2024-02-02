import axios, { AxiosError } from 'axios';
import React, { useEffect, useState, useCallback } from 'react';
import styled from 'styled-components';
import { useToast } from '../components/ToastContext';
import Uploads from '../components/Uploads';
import { useUpload } from '../components/UploadContext';
import LoadingBlur from '../components/LoadingBlur';
import { dirNormalizer, strArrToPath } from '../utils/driveUtils';
import DriveFileTable from '../components/DriveFileTable';
import DriveControl from '../components/DriveControls';

const WrapperStyles = styled.div`
  width: 100%;
  height: 100vh;
  display: grid;
  grid-template-columns: 1fr auto;
`;

const DriveWrapperStyles = styled.div`
  overflow-y: auto;
`;

const DriveStyles = styled.div`
  padding: 2rem;
  box-sizing: border-box;
  display: grid;
  gap: 2rem;
  grid-template-rows: auto auto 1fr;
  font-size: 1.75rem;
`;

const PathStyles = styled.div`
  width: 100%;
  padding: 1rem;
  background: linear-gradient(to right, var(--triadic1-hue), var(--darker));
  box-sizing: border-box;
`;

export default function DrivePage(): JSX.Element {
  const [fetchState, setFetchState] = useState<FetchState>('idle');
  const [path, setPath] = useState<string[]>([]);
  const [files, setFiles] = useState<DriveElement[]>([]);

  const launchToast = useToast();
  const { addRefreshCallback, removeRefreshCallback } = useUpload();

  const fetchFiles = useCallback(() => {
    if (fetchState !== 'pending') {
      setFetchState('pending');
      axios
        .get<DriveElement[]>(`/api/directory?path=/`)
        .then(res => res.data.filter(d => d).map(dirNormalizer))
        .then(data => {
          setFiles(data);
          setFetchState('success');
        })
        .catch((err: AxiosError) => {
          setFetchState('failed');
          launchToast('error', err.message);
        });
    }
  }, [fetchState, launchToast]);

  // eslint-disable-next-line react-hooks/exhaustive-deps
  useEffect(fetchFiles, []);

  useEffect(() => {
    addRefreshCallback(fetchFiles);
    return () => removeRefreshCallback(fetchFiles);
  }, [fetchFiles, addRefreshCallback, removeRefreshCallback]);

  const goToDir = useCallback(
    (dirname: string) => setPath(oldPath => oldPath.concat([dirname])),
    []
  );

  const goToParentDir = useCallback(() => setPath(oldPath => oldPath.slice(0, -1)), []);

  const deleteFileFactory = useCallback(
    (id: string) => () => setFiles(oldFiles => oldFiles.filter(f => f.id !== id)),
    []
  );

  return (
    <WrapperStyles>
      <DriveWrapperStyles>
        <LoadingBlur fetchState={fetchState}>
          <DriveStyles>
            <PathStyles>{strArrToPath(path)}</PathStyles>
            <DriveControl path={path} />
            <DriveFileTable
              path={path}
              files={files}
              goToDir={goToDir}
              goToParentDir={goToParentDir}
              deleteFileFactory={deleteFileFactory}
            />
          </DriveStyles>
        </LoadingBlur>
      </DriveWrapperStyles>
      <Uploads />
    </WrapperStyles>
  );
}
