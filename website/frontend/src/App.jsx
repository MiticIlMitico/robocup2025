import React from 'react';
import Layout from './components/Layout';
import Home from './pages/Home';
import CentralHome from './pages/CentralHome';
import Team from './pages/Team';
import Project from './pages/Project';
import Footer from './components/Footer';
import ChiSiamo from './pages/ChiSiamo';

function App() {
  return (
    <Layout>
      <CentralHome />
      <Project />
      <ChiSiamo />
      <Team />
      <Footer />
    </Layout>
  );
}

export default App;