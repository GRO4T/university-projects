import axios, { AxiosError } from 'axios';
import { Link, navigate } from 'gatsby';
import React, { useState } from 'react';
import LoadingBlur from '../components/LoadingBlur';
import TextInput from '../components/TextInput';
import { useToast } from '../components/ToastContext';
import useForm from '../hooks/useForm';
import AuthStyles from '../styles/AuthStyles';
import FormStyles from '../styles/FormStyles';

export default function RegisterPage(): JSX.Element {
  const [fetchState, setFetchState] = useState<FetchState>('idle');

  const launchToast = useToast();

  const { inputs, handleChange } = useForm({
    email: '',
    username: '',
    password: '',
  });

  const handleSubmit = () => {
    if (fetchState !== 'pending') {
      setFetchState('pending');
      axios
        .post('/api/auth/signup', inputs)
        .then(() => {
          launchToast('success', 'Account created! You can now log in!');
          void navigate('/app/login');
        })
        .catch((err: AxiosError) => {
          launchToast('error', err.response ? err.response.data : err.message);
          setFetchState('failed');
        });
    }
  };

  return (
    <div>
      <LoadingBlur fetchState={fetchState}>
        <AuthStyles className="card">
          <header>Create an account</header>
          <FormStyles onSubmit={handleSubmit}>
            <TextInput type="email" name="email" value={inputs.email} onChange={handleChange} />
            <TextInput
              type="text"
              name="username"
              value={inputs.username}
              onChange={handleChange}
            />
            <TextInput
              type="password"
              name="password"
              value={inputs.password}
              onChange={handleChange}
            />
            <button type="submit">Sign Up</button>
          </FormStyles>
          <footer>
            <Link to="/app/login">Already have an account?</Link>
          </footer>
        </AuthStyles>
      </LoadingBlur>
    </div>
  );
}
