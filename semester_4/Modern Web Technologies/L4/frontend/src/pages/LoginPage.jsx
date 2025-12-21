import React from 'react';
import AuthForm from '../components/Auth/AuthForm';

const LoginPage = () => {
    return (
        <div>
            <h1>Login</h1>
            <AuthForm isLogin={true} />
        </div>
    );
};

export default LoginPage;