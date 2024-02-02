import React, { useState } from 'react';

interface UseForm<T> {
  inputs: T;
  handleChange: (e: React.ChangeEvent<HTMLInputElement>) => void;
  resetForm: () => void;
}

export default function useForm<T>(initial: T): UseForm<T> {
  const [inputs, setInputs] = useState<T>(initial);

  const handleChange = (e: React.ChangeEvent<HTMLInputElement>) => {
    const { value, name } = e.target;

    setInputs(oldInputs => ({
      ...oldInputs,
      [name]: value,
    }));
  };

  const resetForm = () => setInputs(initial);

  return {
    inputs,
    handleChange,
    resetForm,
  };
}
