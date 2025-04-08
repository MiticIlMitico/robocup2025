import React from 'react';
import FadeInOnScroll from './components/FadeInOnScroll';
import CentralHome from './pages/CentralHome';
import Team from './pages/Team';
import Project from './pages/Project';
import Footer from './components/Footer';
import ChiSiamo from './pages/ChiSiamo';
import GalleryCircuiti from './components/GalleryCircuiti';

function App() {
  return (<>
      <CentralHome />
      <div id="project-section">
        <Project />
      </div>
      <FadeInOnScroll>
      <ChiSiamo />
</FadeInOnScroll>
      <FadeInOnScroll>
        <GalleryCircuiti
          images={[
            "/circuito-re-julien.png",
            "/circuito-alex.png",
            "/circuito-pinguini.png",
            "/wiring-arduino.png"
          ]}
          height="h-64" // oppure "h-48", "h-80" se vuoi variarlo
        />
      </FadeInOnScroll>
      
        <Team />
        
      <Footer />
  </>
  );
}

export default App;
