// Register.jsx
import React from 'react';
import AuthForm from './AuthForm';

const Register = () => {
  return (
    <div className="auth-page">
      <AuthForm isLogin={false} />
    </div>
  );
};

export default Register;