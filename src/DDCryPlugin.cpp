// Framework include files

#include "DDCryPlugin.h"

#include <DD4hep/Printout.h>
#include <DDG4/Geant4Primary.h>
#include <DDG4/Geant4Context.h>
#include <DDG4/Factories.h>

#include <G4ParticleTable.hh>

#include <CRYGenerator.h>
#include <CRYSetup.h>

#include <G4Event.hh>

using namespace dd4hep;
using namespace dd4hep::sim;
typedef dd4hep::detail::ReferenceBitMask<int> PropertyMask;

DECLARE_GEANT4_EVENT_READER(CRYEventReader)

/// Initializing constructor
CRYEventReader::CRYEventReader(const std::string& configFilePath)
  : Geant4EventReader(configFilePath)
{
  m_configFilePath = configFilePath;
}

/// Default destructor
CRYEventReader::~CRYEventReader()   {
}


/// Read an event and fill a vector of MCParticles.
CRYEventReader::EventReaderStatus
CRYEventReader::readParticles(int,
                              Vertices& vertices,
                              std::vector<Particle*>& particles)
{
   
  // Generate an event
  std::vector<CRYParticle*> *ev = new std::vector<CRYParticle*>;

  m_gen->genEvent(ev);

  // Write the event information to particles
  for ( unsigned j=0; j<ev->size(); j++) {
    CRYParticle* mcp = (*ev)[j];

    Geant4ParticleHandle p(new Particle(j));
    const double ke     = mcp->ke() * CLHEP::MeV;
    
    p->pdgID        = mcp->PDGid();
    p->charge       = mcp->charge();
    p->psx          = mcp->u() * ke;
    p->psy          = mcp->v() * ke;
    p->psz          = mcp->w() * ke;
    p->time         = mcp->t()*CLHEP::s;
    p->vsx          = mcp->x() * CLHEP::m;
    p->vsy          = mcp->y() * CLHEP::m;
    p->vsz          = mcp->z() * CLHEP::m;
    // p->vex          = vex[0]*CLHEP::mm;
    // p->vey          = vex[1]*CLHEP::mm;
    // p->vez          = vex[2]*CLHEP::mm;
    p->process      = 0;
    p->mass         = p.definition()->GetPDGMass();

    //Fixme?
    PropertyMask status(p->status);
    status.set(G4PARTICLE_GEN_STABLE);

    if ( p->parents.size() == 0 )  {

      Geant4Vertex* vtx = new Geant4Vertex ;
      vertices.emplace_back( vtx );
      vtx->x = p->vsx;
      vtx->y = p->vsy;
      vtx->z = p->vsz;
      vtx->time = p->time;

      vtx->out.insert(p->id) ;
    }
    particles.emplace_back(p);
    //cleanup
    delete (*ev)[j];
  }

  //cleanup
  delete ev;

  return EVENT_READER_OK;
}

