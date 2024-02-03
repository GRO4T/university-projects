import axios, { AxiosError } from 'axios';
import React, { createContext, useMemo, useContext, useState, useCallback, useEffect } from 'react';
import { useToast } from './ToastContext';
import { getNewUIDGenerator } from '../utils/simpleUID';
import { useAuth } from './AuthContext';
import { axiosUpload } from '../utils/uploadService';
import { throwNoProvider } from '../utils/utils';

export interface UploadManager {
  upload: (files: FileList, path: string) => void;
  refresh: () => void;
  addRefreshCallback: (cb: () => void) => void;
  removeRefreshCallback: (cb: () => void) => void;
  clearCompleted: () => void;
}

const UploadContext = createContext<UploadManager>({
  upload: () => throwNoProvider('Upload'),
  refresh: () => throwNoProvider('Upload'),
  addRefreshCallback: () => throwNoProvider('Upload'),
  removeRefreshCallback: () => throwNoProvider('Upload'),
  clearCompleted: () => throwNoProvider('Upload'),
});

const UploadFilesContext = createContext<UploadFile[]>([]);

export const useUpload = (): UploadManager => useContext(UploadContext);
export const useUploadFiles = (): UploadFile[] => useContext(UploadFilesContext);

interface UploadContextWrapperProps {
  children: JSX.Element | JSX.Element[];
}

export function UploadContextWrapper({ children }: UploadContextWrapperProps): JSX.Element {
  const [files, setFiles] = useState<UploadFile[]>([]);
  const [refreshCallbacks, setRefreshCallbacks] = useState<Array<() => void>>([]);
  const [needsRefreshing, setNeedsRefreshing] = useState<boolean>(false);

  const generateUID = useMemo(getNewUIDGenerator, []);

  const launchToast = useToast();
  const authService = useAuth();

  const updateFileFactory = (id: number) => (getUpdated: (oldFile: UploadFile) => UploadFile) => {
    setFiles(oldFiles => {
      const oldFile = oldFiles.find(f => f.id === id);
      if (oldFile) {
        return oldFiles
          .filter(f => f.id !== id)
          .concat([getUpdated(oldFile)])
          .sort((a, b) => b.id - a.id);
      }
      return oldFiles;
    });
  };

  const upload = useCallback(
    (fileList: FileList, path: string) => {
      Array.from(fileList).forEach(file => {
        const data = new FormData();
        data.append('file', file);

        const uploadFile: UploadFile = {
          id: generateUID(),
          name: file.name,
          path,
          status: 'pending',
          loaded: 0,
          total: file.size,
          cancelTokenSource: axios.CancelToken.source(),
        };

        setFiles(oldFiles => oldFiles.concat([uploadFile]).sort((a, b) => b.id - a.id));

        const updateFile = updateFileFactory(uploadFile.id);

        const onCompleted = () => {
          setNeedsRefreshing(true);
          launchToast('success', `Uploaded ${uploadFile.name}`);
        };

        const onError = (err: AxiosError) => launchToast('error', err.message);

        void axiosUpload(data, uploadFile, updateFile, onCompleted, onError);
      });
    },
    [generateUID, launchToast]
  );

  const clearAllAndCancel = useCallback(
    () =>
      setFiles(oldFiles => {
        oldFiles.forEach(f => f.cancelTokenSource.cancel(`Upload of ${f.name} cancelled!`));
        return [];
      }),
    []
  );

  useEffect(() => {
    if (needsRefreshing) {
      refreshCallbacks.forEach(cb => cb());
      setNeedsRefreshing(false);
    }
  }, [needsRefreshing, refreshCallbacks]);

  useEffect(clearAllAndCancel, [authService, clearAllAndCancel]);

  const uploadManager: UploadManager = useMemo(
    () => ({
      upload,
      refresh: () => setNeedsRefreshing(true),
      addRefreshCallback: cb => setRefreshCallbacks(oldCbs => [...oldCbs, cb]),
      removeRefreshCallback: cb =>
        setRefreshCallbacks(oldCbs => oldCbs.filter(oldCb => oldCb !== cb)),
      clearCompleted: () => setFiles(oldFiles => oldFiles.filter(f => f.status === 'pending')),
    }),
    [upload]
  );

  return (
    <>
      <UploadContext.Provider value={uploadManager}>
        <UploadFilesContext.Provider value={files}>{children}</UploadFilesContext.Provider>
      </UploadContext.Provider>
    </>
  );
}
