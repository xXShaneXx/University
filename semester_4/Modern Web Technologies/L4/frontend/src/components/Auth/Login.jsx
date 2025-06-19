// Login.jsx
import React from 'react';
import AuthForm from './AuthForm';
import { useLocation } from 'react-router-dom';

const Login = () => {
  const location = useLocation();
  const successMessage = location.state?.success;

  return (
    <div className="auth-page">
      {successMessage && <div className="success-message">{successMessage}</div>}
      <AuthForm isLogin={true} />
    </div>
  );
};

export default Login;