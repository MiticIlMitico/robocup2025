import React from 'react';

const Layout = ({ children }) => {
  return (
    <div 
      className="min-h-screen bg-cover bg-center bg-fixed bg-no-repeat"
      style={{ backgroundImage: "url('/path/to/your/background-image.jpg')" }}
    >
      {children}
    </div>
  );
};

export default Layout;