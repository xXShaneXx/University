import React from 'react';
import { BrowserRouter as Router, Routes, Route } from 'react-router-dom';
import { AuthProvider } from './context/AuthContext';
import { ReviewProvider } from './context/ReviewContext';
import Home from './pages/HomePage';
import Login from './components/Auth/Login';
import Register from './components/Auth/Register';
import Dashboard from './pages/Dashboard';
import AdminPanel from './pages/AdminPanel';
import ProductList from './components/Products/ProductList';
import ProductForm from './components/Products/ProductForm';
import ProtectedRoute from './components/ProtectedRoute';
import ReviewPage from './pages/ReviewPage';
import Navbar from './components/Navbar';
import NotFound from './pages/NotFound';

function App() {
  return (
    <Router>
      <AuthProvider>
        <ReviewProvider>
          <Navbar />
          <Routes>
            <Route path="/" element={<Home />} />
            <Route path="/login" element={<Login />} />
            <Route path="/register" element={<Register />} />

            <Route path="/dashboard" element={
              <ProtectedRoute>
                <Dashboard />
              </ProtectedRoute>
            } />

            <Route path="/products" element={
              <ProtectedRoute>
                <ProductList />
              </ProtectedRoute>
            } />

            <Route path="/products/add" element={
              <ProtectedRoute roles={['admin', 'user']}>
                <ProductForm />
              </ProtectedRoute>
            } />

            <Route path="/products/edit/:id" element={
              <ProtectedRoute roles={['admin', 'user']}>
                <ProductForm />
              </ProtectedRoute>
            } />

            <Route path="/products/:productId/reviews" element={
              <ProtectedRoute>
                <ReviewPage />
              </ProtectedRoute>
            } />

            <Route path="/admin" element={
              <ProtectedRoute roles={['admin']}>
                <AdminPanel />
              </ProtectedRoute>
            } />

            <Route path="*" element={<NotFound />} />
          </Routes>
        </ReviewProvider>
      </AuthProvider>
    </Router>
  );
}

export default App;