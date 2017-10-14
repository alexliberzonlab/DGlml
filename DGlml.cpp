/*-----------------------------------------------------------------------

  File        : DGlml.cpp

  Description : particle Displacement Generator of the Laboratory of Mechanic
s in Lille (LML)

  Author      : Sebastien COUDERT
  
  --------------------------------------------------------------------*/

/*Additional documentation for the generation of the reference page (using doxyg
en)*/
/** \mainpage
 *
 *  index:
 *  \li \ref sectionCommandLine
 *  \li \ref sectionDGlmlDocumentation
 *  \li \ref sectionDoxygenSyntax
 *
 *  \section sectionCommandLine command line options
 *  \verbinclude "DGlml.help.output"
 *
 *  \section sectionDGlmlDocumentation documentation outline
 *  This is the reference documentation of <a href="http://www.meol.cnrs.fr/">PI
Vlml</a>, from the <a href="http://www.univ-lille1.fr/lml/">LML</a>.\n\n
 *  PIVlml is a PIV image processing software. The program begins in the main fu
nction in the <a href="_p_i_vlml_8cpp.html#a1">PIVlml.cpp</a> source file.\n\n
 *  This documentation has been automatically generated from the PIVlml sources,
 
 *  using the tool <a href="http://www.doxygen.org">doxygen</a>. It should be re
aded as HTML, LaTex and man page.\n
 *  It contains both
 *  \li a detailed description of all classes and functions
 *  \li a user guide (cf. \ref pages.html "related pages")
 *
 *  that as been documented in the sources.
 *
 *  \par Additional needed libraries:
 *
 *  \li <a href="http://cimg.sourceforge.net">the CImg Library</a> using <a href
="http://www.imagemagick.org/">ImageMagick</a> for a few image format
 *
 *
 *  \section sectionDoxygenSyntax make documentation using Doxygen syntax
 *  Each function in the source code should be commented using \b doxygen \b syn
tax in the same file.
 *  The documentation need to be written before the function.
 *  The basic syntax is presented in this part.
 *  \verbinclude "doxygen.example1.txt"
 *
 *  Two kind of comments are needed for \b declaration and \b explanation \b par
ts of the function:
 *  Standart documentation should the following (\b sample of code documentation
):
 *  \verbinclude "doxygen.example2.txt"
 *
 *  In both declaration and explanation part, \b writting and \b highlithing syn
tax can be the following:\n\n
 *  \li \c \\code to get\n
 *
 *  \li \c \\n    a new line
 *  \li \c \\li   a list (dot list)
 *
 *  \li \c \\b    bold style
 *  \li \c \\c    code style
 *  \li \c \\e    enhanced style (italic)
 *
 *  For making \b shortcut please use:\n
 *  \li \c \\see to make a shortcut to a related function or variable
 *  \li \c \\link to make a shortcut to a file or a function
 *  \note this keyword needs to be closed using \c \\end*
 *
 *  \li \c \\todo to add a thing to do in the list of <a href="./todo.html">oDo</
a> for the whole program
 *
 *  In explanation part, \b paragraph style can be the following:\n
 *  \li \c \\code for an example of the function use
 *  \li \c \\note to add a few notes
 *  \li \c \\attention for SOMETHING NOT FULLY DEFINED YET
 *  \li \c \\warning to give a few warning on the function
 *  \note these keywords need to be closed using \c \\end*
 *
 *  \verbinclude "doxygen.example3.txt"
 *
 *  Many other keywords are defined, so please read the documentation of <a href
="http://www.doxygen.org/commands.html">doxygen</a>.
 *
**/

#include<iostream>

//debug display (0- quiet, 1- only final image, 2- image step by step)
//#define cimg_debug 0
//#define cimg_debug 1
#define cimg_debug 2
#include "../CImg/CImg.h"

using namespace cimg_library;
using namespace std;

//The lines below are necessary when using a non-standard compiler as visualcpp6.
#ifdef cimg_use_visualcpp6
#define std
#endif
#ifdef min
#undef min
#undef max
#endif
//end visualcpp6

//create random position particles
/* create particle parameters with random generator:
 * particle parameters: position x and y
 * \param[out] particles: all particle parameters
 * \param[in] n: number of particles to create
 * \param[in] min_x: minimum of the particle position range along x axis
 * \param[in] max_x: maximum of the particle position range along x axis
 * \param[in] min_y: minimum of the particle position range along y axis
 * \param[in] max_y: maximum of the particle position range along y axis
**/
template<typename T> int create_random_parameters(CImg<T> &particles/*gaussian particles: position x,y*/,
  /*number*/int n,
  /*positions*/T min_x,T max_x,T min_y,T max_y)
{
#define POS_X 0
#define POS_Y 1
  CImg<float> coord_x(n),coord_y(n);
//create random particle parameters: 2D gaussian function
  ///2D position of the maximum (i.e. particle position)
  coord_x.rand(min_x,max_x);coord_y.rand(min_y,max_y);
  particles.assign(n,1,1,2);
  cimg_forX(particles,i)
  {
    particles(i,0,0,POS_X)=coord_x(i);
    particles(i,0,0,POS_Y)=coord_y(i);
  }
  return 0;
}//create_random_parameters

//read particle parameters from stdin
/* get gaussian particle parameters from stdin
 * particle parameters: position x and y, sigma and level of 2D gaussian function
 * \param[out] n: number of particles
 * \param[out] particles: all particle parameters
**/
template<typename T> int get_particles(int &n,CImg<T> &particles)
{
  if(n<1) n=512;
  //positions x and y, sigma and level parameter arrays
  CImg<T> coord_x(n),coord_y(n),psigma(n),color(n);

//get particle parameters from stdin
  int p=-1;n--;
  float x,y,q,w;
  while(cin>>x>>y>>q>>w)
  {
//cout<<endl<<"("<<x<<","<<y<<")";
    p++;if(p>n) {cout<<endl<<"information: not enough space in the particle array (i.e. reallocating more space), set -n option to speed up execution (n should be higher than "<<n+1<<")."<<flush;n+=512;coord_x.resize(n);coord_y.resize(n);psigma.resize(n),color.resize(n);n--;}
    coord_x(p)=x;
    coord_y(p)=y;
    psigma(p)=q;
    color(p)=w;
  }
  if(p<0) return -1;
  //set number of particles
  n=p+1;
  //resize x and y arrays
  coord_x.crop(0,n-1);coord_y.crop(0,n-1);psigma.crop(0,n-1);color.crop(0,n-1);
//set particle parameters in only one structure (i.e. one CImg object)
  particles.assign(n,1,1,4);
  particles.draw_image(0,0,0,0,coord_x);
  particles.draw_image(0,0,0,1,coord_y);
  particles.draw_image(0,0,0,2,psigma);
  particles.draw_image(0,0,0,3,color);
  return 0;
}//get_particles

//! draw particle position within the image
/* create single pixel particles within the image to show position only.
 * \param[in/out] image: particle image
 * \param[in] particles: all particle parameters
 * \param[in] radius: radius of the disc that represents a particle
**/
template<typename imageT, typename T> int draw_particle_position(CImg<imageT> &image, CImg<T> &particles,int radius=3)
{
#define POS_X 0
#define POS_Y 1
  imageT color=cimg::type<imageT>::max();
//set background to 0
  image.fill(0);
//draw particles on the image
  cimg_forX(particles,i)
  {
    int x=(int)particles(i,0,0,POS_X);
    int y=(int)particles(i,0,0,POS_Y);
    if( (x>-radius) && x<image.dimx()+radius && (y>-radius) && y<image.dimy()+radius ) image.draw_circle(x,y,radius,&color);
  }
  return 0;
}//draw_particle_position



//! displace particle using a uniform translation
/* For each particle, this function add a uniform translation to its position.
 * for x position px: px=px+dx*displacement_type
 * e.g. displacement_type=-0.5 px=px-dx/2 (i.e. exposure 1)
 * for y position py: py=py+dy*displacement_type
 * e.g. displacement_type=+0.5 py=py+dy/2 (i.e. exposure 2)
 * \param[in/out] particles: all particle parameters
 * \param[in] displacement_type: type of displacement to generate (should be -0.5 or 0.5)
 * \param[in] dx: displacement along x axis
 * \param[in] dy: displacement along y axis
**/
template<typename T> int displacement_translation(CImg<T> &particles,T displacement_type,T dx,T dy)
{
#define POS_X 0
#define POS_Y 1
  if(particles.dimv()<2) return 1;
  dx*=displacement_type;
  dy*=displacement_type;
  cimg_forX(particles,p)
  {
    particles(p,0,0,POS_X)+=dx;
    particles(p,0,0,POS_Y)+=dy;
  }
  return 0;
}//displacement_translation

//! create a displacement map using plasma function
/* Creation of a displacement map from a 3 components plasma function (i.e. looks like a turbulent flow, but no fluid mechanics law in it).
 * \note mainly for testing purpose.
 * \param[out] plasma: 3C displacement map
 * \param[in] dimx: size of the map
 * \param[in] dimy: size of the map
 * \param[in] velocity_min: minimum displacement along any axis (e.g. -6.5 pixel)
 * \param[in] velocity_max: maximum displacement along any axis (e.g. +6.5 pixel)
 * \param[in] structure_size: bluring size (i.e. kind of structure size such as vorticies)
**/
template<typename T> int create_plasma(CImg<T> &plasma,int dimx,int dimy,T velocity_min,T velocity_max,T structure_size)
{
#define VEL_X 0
#define VEL_Y 1
#define VEL_Z 0
  int nb_components=3;
  plasma.assign(dimx,dimy,1,nb_components,0).noise(100).draw_plasma();
  cimg_forV(plasma,k) plasma.get_shared_channel(k).blur((float)(cimg::rand()*structure_size)).normalize(velocity_min,velocity_max);
#if cimg_debug>1
    plasma.display("fake turbulence velocity (plasma) only DGlml");
#endif
  return 0;
}//create_plasma

//! displace particle using a displacement map
/* Each particle is displaced by the amount of the nearest pixel of the displacement map.
 * for y position py: py=py+map(x,y)*displacement_type
 * e.g. displacement_type=+0.5 py=py+dy/2 (i.e. exposure 2)
 * \param[in/out] particles: all particle parameters
 * \param[in] displacement_type: type of displacement to generate (should be -0.5 or 0.5)
 * \param[in] map: 2C displacement map
**/
template<typename T,typename mapT> int displacement_map(CImg<T> &particles,mapT displacement_type,CImg<mapT> &map)
{
#define POS_X 0
#define POS_Y 1
#define VEL_X 0
#define VEL_Y 1
  if(particles.dimv()<2) return 1;
  if(map.dimv()<2) return 1;
  cimg_forX(particles,p)
  {
    T x=particles(p,0,0,POS_X),y=particles(p,0,0,POS_Y);
    particles(p,0,0,POS_X)+=map((int)x,(int)y,0,VEL_X)*displacement_type;
    particles(p,0,0,POS_Y)+=map((int)x,(int)y,0,VEL_Y)*displacement_type;
  }
  return 0;
}//displacement_map

int main(int argc,char **argv)
{
//commmand line options
 ///usage
  cimg_usage(std::string("particle Displacement Generator of LML, \
it uses different GNU libraries (see --info option)\n\n \
usage: ./DGlml -h -I #help and compilation information\n \
       ./DGlml --test -O true #just check: random gaussian particle parameters\n \
         PGlml -o particle.cimg #generate particle parameters using PGlml of SIGlml program suite\n \
       ./DGlml -i particle.cimg -d -0.5 -o particle_exposure1.cimg -O true #exposure 1 position rendering and show position image\n \
       ./DGlml -i particle.cimg -d +0.5 -o particle_exposure2.cimg -O true #exposure 2 position rendering and show position image\n \
version: "+std::string(VERSION)+"\t(other library versions: DGlml_parameter_format."+std::string(PG_FORMAT_VERSION)+")\n compilation date: " \
            ).c_str());//cimg_usage
  ///information and help
  const bool show_h   =cimg_option("-h",    false,NULL);//-h hidden option
        bool show_help=cimg_option("--help",show_h,"help (or -h option)");show_help=show_h|show_help;//same --help or -h option
  bool show_info=cimg_option("-I",false,NULL);//-I hidden option
  if( cimg_option("--info",show_info,"show compilation options (or -I option)") ) {show_info=true;cimg_library::cimg::info();}//same --info or -I option  // Usage of the program displayed at the command line
//tests
  cimg_help("\nTest options");
  const bool test = cimg_option("--test",false,"Run and show displacement test (add -O option to see resulting image).");
//displacement
  cimg_help("\nDisplacement options");
  float displacement_type= cimg_option("-d",0.0,"displacement: 0 none, 1 double exposure, +/-0.5 single exposure 1/2.");
  const char* displacement_function=cimg_option("--function","translation","displacement function: translation or plasma.");
  float displacement_constant_x= cimg_option("--dx",12.345,"constant displacement value along x axis or along any direction.");
  float displacement_constant_y= cimg_option("--dy",12.345,"constant displacement value along y axis or structure size.");
//particles
  cimg_help("\nParticle options");
  ///particle parameter from stdin
  const char* particleInputType= cimg_option("-i","stdin","particle parameters source cimg file or can be set to stdin (external) (e.g. echo 31.234 12.345 1.5 255 | ./DGlml -i stdin)");
  const char* particleOutputType= cimg_option("-o","stdout","particle parameters destination cimg file or can be set to stdout (external) (e.g. echo 32 16 2.3 200 | ./DGlml --function translation -d 0.5 --dx 12.345 --dy 1.234 -i stdin -o stdout)");
  ///particle number
  int nbParticles= cimg_option("-n",-1,"to limit the number of particles");
//image
  cimg_help("\nParticle position image options");
  ///image size
  int option_image_width = cimg_option("-W",512,"image width");
  int option_image_height= cimg_option("-H",512,"image height");
  ///position image file name (optional output)
  const bool option_image=cimg_option("-O",false,"display position image");
  const int option_image_particle_radius=cimg_option("-r",3,"particle radius of position image");
  const char* option_image_filename=cimg_option("-P","false","file name to output positions in an image (e.g. -P positions.png)");
  const bool option_image_file=cimg::strcmp(option_image_filename,"false");
  ///stop if help requested
  if(show_help) {/*print_help(std::cerr);*/return 0;}

//get particle parameters (input)
///create parameter array
  CImg<float> particles;
///create parameters if testing
  if(test)
  {
    cerr<<"\rinformation: create particles with random positions."<<flush;
    create_random_parameters(particles,(nbParticles==-1)?1234:nbParticles,
                             (float)0,(float)option_image_width-1,(float)0,(float)option_image_height-1);
///read from stdin
  }
  else if (!cimg::strcmp("stdin",particleInputType))
  {
    cerr<<"\rinformation: get particles from stdin (must be 4 parameters: x,y,sigma,level)."<<flush;
    get_particles(nbParticles,particles);
  }
///read from file
  else 
  {
    cerr<<"\rinformation: get particles from file \""<<particleInputType<<"\"."<<flush;
    int error;
    if(!(error=particles.load(particleInputType))) return error;
    if(particles.dimv()<4) {cerr<<"\nerror: needs at least 4 parameters for a gaussian particle (file \""<<particleInputType<<"\" do NOT."<<flush;return 1;}
  }
//displacement generation
  if(displacement_type==0) cerr<<"\rinformation: no displacement generated."<<flush;
  else if(displacement_type==1)
  {
    cerr<<"\rinformation: double exposure generation."<<flush;
    CImg<float> particles2(particles);
    if (!cimg::strcmp("translation",displacement_function))
    {
      displacement_translation(particles,-0.5f,displacement_constant_x,displacement_constant_y);
      displacement_translation(particles2,0.5f,displacement_constant_x,displacement_constant_y);
    }
    if (!cimg::strcmp("plasma",displacement_function))
    {
      CImg<> plasma;
      create_plasma(plasma,option_image_width,option_image_height,-displacement_constant_x,displacement_constant_x,displacement_constant_y);
      displacement_map(particles,-0.5f,plasma);
      displacement_map(particles2,0.5f,plasma);
    }
    particles.append(particles2,'x');
  }
  else
  {
    cerr<<"\rinformation: single exposure generation."<<flush;
    if (!cimg::strcmp("translation",displacement_function)) displacement_translation(particles,displacement_type,displacement_constant_x,displacement_constant_y);
    if (!cimg::strcmp("plasma",displacement_function))
    {
      CImg<> plasma;
      create_plasma(plasma,option_image_width,option_image_height,-displacement_constant_x,displacement_constant_x,displacement_constant_y);
      displacement_map(particles,displacement_type,plasma);
    }
  }
//set particle parameters (ouput)
  if(!cimg::strcmp("stdout",particleOutputType)) cimg_forX(particles,p) {cimg_forV(particles,v) cout<<particles(p,0,0,v)<<"\t"; cout<<endl;}
  else particles.save(particleOutputType);
#if cimg_debug>1
    particles.display("particle parameters DGlml");
#endif

//create particle position image (optional)
  if(option_image||option_image_file)
  {
  //create image
    CImg<unsigned char> image(option_image_width,option_image_height,1);
  //draw particles within the image
    draw_particle_position(image,particles,option_image_particle_radius);
  //save
  ///08bit
    if(option_image_file) image.save(option_image_filename);
#if cimg_debug>0
    if(option_image) image.display("particle position only DGlml");
#endif
  }
 return 0;
}//main

