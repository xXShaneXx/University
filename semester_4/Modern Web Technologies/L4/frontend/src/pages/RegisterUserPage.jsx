import React from 'react';
import AuthForm from '../components/Auth/AuthForm';

const RegisterUserPage = () => {
  return (
    <div>
      <h1>Register</h1>
      <AuthForm isRegister={true} />
    </div>
  );
};

export default RegisterUserPage;