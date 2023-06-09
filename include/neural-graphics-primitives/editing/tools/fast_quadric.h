/////////////////////////////////////////////
//
// Mesh Simplification Tutorial
//
// (C) by Sven Forstmann in 2014
//
// License : MIT
// http://opensource.org/licenses/MIT
//
//https://github.com/sp4cerat/Fast-Quadric-Mesh-Simplification
//
// 5/2016: Chris Rorden created minimal version for OSX/Linux/Windows compile

# pragma once

#include <Eigen/Dense>

//#include <iostream>
//#include <stddef.h>
//#include <functional>
//#include <sys/stat.h>
//#include <stdbool.h>
#include <string.h>
//#include <ctype.h>
//#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <vector>
#include <string>
#include <math.h>
#include <float.h> //FLT_EPSILON, DBL_EPSILON

#define loopi(start_l,end_l) for ( int i=start_l;i<end_l;++i )
#define loopi(start_l,end_l) for ( int i=start_l;i<end_l;++i )
#define loopj(start_l,end_l) for ( int j=start_l;j<end_l;++j )
#define loopk(start_l,end_l) for ( int k=start_l;k<end_l;++k )

namespace Simplification {
    struct vector3
    {
    double x, y, z;
    };

    struct vec3f
    {
        double x, y, z;

        inline vec3f( void ) {}

        //inline vec3f operator =( vector3 a )
        // { vec3f b ; b.x = a.x; b.y = a.y; b.z = a.z; return b;}

        inline vec3f( vector3 a )
        { x = a.x; y = a.y; z = a.z; }

        inline vec3f( const double X, const double Y, const double Z )
        { x = X; y = Y; z = Z; }

        inline vec3f operator + ( const vec3f& a ) const
        { return vec3f( x + a.x, y + a.y, z + a.z ); }

        inline vec3f operator += ( const vec3f& a ) const
        { return vec3f( x + a.x, y + a.y, z + a.z ); }

        inline vec3f operator * ( const double a ) const
        { return vec3f( x * a, y * a, z * a ); }

        inline vec3f operator * ( const vec3f a ) const
        { return vec3f( x * a.x, y * a.y, z * a.z ); }

        inline vec3f v3 () const
        { return vec3f( x , y, z ); }

        inline vec3f operator = ( const vector3 a )
        { x=a.x;y=a.y;z=a.z;return *this; }

        inline vec3f operator = ( const vec3f a )
        { x=a.x;y=a.y;z=a.z;return *this; }

        inline vec3f operator / ( const vec3f a ) const
        { return vec3f( x / a.x, y / a.y, z / a.z ); }

        inline vec3f operator - ( const vec3f& a ) const
        { return vec3f( x - a.x, y - a.y, z - a.z ); }

        inline vec3f operator / ( const double a ) const
        { return vec3f( x / a, y / a, z / a ); }

        inline double dot( const vec3f& a ) const
        { return a.x*x + a.y*y + a.z*z; }

        inline vec3f cross( const vec3f& a , const vec3f& b )
        {
            x = a.y * b.z - a.z * b.y;
            y = a.z * b.x - a.x * b.z;
            z = a.x * b.y - a.y * b.x;
            return *this;
        }

        inline double angle( const vec3f& v )
        {
            vec3f a = v , b = *this;
            double dot = v.x*x + v.y*y + v.z*z;
            double len = a.length() * b.length();
            if(len==0)len=0.00001f;
            double input = dot  / len;
            if (input<-1) input=-1;
            if (input>1) input=1;
            return (double) acos ( input );
        }

        inline double angle2( const vec3f& v , const vec3f& w )
        {
            vec3f a = v , b= *this;
            double dot = a.x*b.x + a.y*b.y + a.z*b.z;
            double len = a.length() * b.length();
            if(len==0)len=1;

            vec3f plane; plane.cross( b,w );

            if ( plane.x * a.x + plane.y * a.y + plane.z * a.z > 0 )
                return (double) -acos ( dot  / len );

            return (double) acos ( dot  / len );
        }

        inline vec3f rot_x( double a )
        {
            double yy = cos ( a ) * y + sin ( a ) * z;
            double zz = cos ( a ) * z - sin ( a ) * y;
            y = yy; z = zz;
            return *this;
        }
        inline vec3f rot_y( double a )
        {
            double xx = cos ( -a ) * x + sin ( -a ) * z;
            double zz = cos ( -a ) * z - sin ( -a ) * x;
            x = xx; z = zz;
            return *this;
        }
        inline void clamp( double min, double max )
        {
            if (x<min) x=min;
            if (y<min) y=min;
            if (z<min) z=min;
            if (x>max) x=max;
            if (y>max) y=max;
            if (z>max) z=max;
        }
        inline vec3f rot_z( double a )
        {
            double yy = cos ( a ) * y + sin ( a ) * x;
            double xx = cos ( a ) * x - sin ( a ) * y;
            y = yy; x = xx;
            return *this;
        }
        inline vec3f invert()
        {
            x=-x;y=-y;z=-z;return *this;
        }
        inline vec3f frac()
        {
            return vec3f(
                x-double(int(x)),
                y-double(int(y)),
                z-double(int(z))
                );
        }

        inline vec3f integer()
        {
            return vec3f(
                double(int(x)),
                double(int(y)),
                double(int(z))
                );
        }

        inline double length() const
        {
            return (double)sqrt(x*x + y*y + z*z);
        }

        inline vec3f normalize( double desired_length = 1 )
        {
            double square = sqrt(x*x + y*y + z*z);
            /*
            if (square <= 0.00001f )
            {
                x=1;y=0;z=0;
                return *this;
            }*/
            //double len = desired_length / square;
            x/=square;y/=square;z/=square;

            return *this;
        }
        static vec3f normalize( vec3f a );

        static void random_init();
        static double random_double();
        static vec3f random();

        static int random_number;

        double random_double_01(double a){
            double rnf=a*14.434252+a*364.2343+a*4213.45352+a*2341.43255+a*254341.43535+a*223454341.3523534245+23453.423412;
            int rni=((int)rnf)%100000;
            return double(rni)/(100000.0f-1.0f);
        }

        vec3f random01_fxyz(){
            x=(double)random_double_01(x);
            y=(double)random_double_01(y);
            z=(double)random_double_01(z);
            return *this;
        }

    };

    class SymetricMatrix {

        public:

        // Constructor

        SymetricMatrix(double c=0) { loopi(0,10) m[i] = c;  }

        SymetricMatrix(	double m11, double m12, double m13, double m14,
                            double m22, double m23, double m24,
                                        double m33, double m34,
                                                    double m44) {
                m[0] = m11;  m[1] = m12;  m[2] = m13;  m[3] = m14;
                            m[4] = m22;  m[5] = m23;  m[6] = m24;
                                        m[7] = m33;  m[8] = m34;
                                                        m[9] = m44;
        }

        // Make plane

        SymetricMatrix(double a,double b,double c,double d)
        {
            m[0] = a*a;  m[1] = a*b;  m[2] = a*c;  m[3] = a*d;
                        m[4] = b*b;  m[5] = b*c;  m[6] = b*d;
                                    m[7 ] =c*c; m[8 ] = c*d;
                                                m[9 ] = d*d;
        }

        double operator[](int c) const { return m[c]; }

        // Determinant

        double det(	int a11, int a12, int a13,
                    int a21, int a22, int a23,
                    int a31, int a32, int a33)
        {
            double det =  m[a11]*m[a22]*m[a33] + m[a13]*m[a21]*m[a32] + m[a12]*m[a23]*m[a31]
                        - m[a13]*m[a22]*m[a31] - m[a11]*m[a23]*m[a32]- m[a12]*m[a21]*m[a33];
            return det;
        }

        const SymetricMatrix operator+(const SymetricMatrix& n) const
        {
            return SymetricMatrix( m[0]+n[0],   m[1]+n[1],   m[2]+n[2],   m[3]+n[3],
                                                m[4]+n[4],   m[5]+n[5],   m[6]+n[6],
                                                            m[ 7]+n[ 7], m[ 8]+n[8 ],
                                                                        m[ 9]+n[9 ]);
        }

        SymetricMatrix& operator+=(const SymetricMatrix& n)
        {
            m[0]+=n[0];   m[1]+=n[1];   m[2]+=n[2];   m[3]+=n[3];
            m[4]+=n[4];   m[5]+=n[5];   m[6]+=n[6];   m[7]+=n[7];
            m[8]+=n[8];   m[9]+=n[9];
            return *this;
        }

        double m[10];
    };
    ///////////////////////////////////////////

    // Global Variables & Strctures
    enum Attributes {
        NONE,
        NORMAL = 2,
        TEXCOORD = 4,
        COLOR = 8
    };
    struct Triangle { int v[3];double err[4];int deleted,dirty,attr;vec3f n;vec3f uvs[3];int material; };
    struct Vertex { vec3f p;int tstart,tcount;SymetricMatrix q;int border;};
    struct Ref { int tid,tvertex; };

    struct FastQuadric
    {
        std::vector<Triangle> triangles;
        std::vector<Vertex> vertices;
        std::vector<Ref> refs;
        std::vector<std::string> materials;

        // Helper functions

        double vertex_error(SymetricMatrix q, double x, double y, double z);
        double calculate_error(int id_v1, int id_v2, vec3f &p_result);
        bool flipped(vec3f p,int i0,int i1,Vertex &v0,Vertex &v1,std::vector<int> &deleted);
        void update_uvs(int i0,const Vertex &v,const vec3f &p,std::vector<int> &deleted);
        void update_triangles(int i0,Vertex &v,std::vector<int> &deleted,int &deleted_triangles);
        void update_mesh(int iteration);
        void compact_mesh();
        //
        // Main simplification function
        //
        // target_count  : target nr. of triangles
        // agressiveness : sharpness to increase the threshold.
        //                 5..8 are good numbers
        //                 more iterations yield higher quality
        //

        void simplify_mesh(int target_count, double agressiveness=7, bool verbose=false)
        {
            // init
            loopi(0,triangles.size())
            {
                triangles[i].deleted=0;
            }

            // main iteration loop
            int deleted_triangles=0;
            std::vector<int> deleted0,deleted1;
            int triangle_count=triangles.size();
            //int iteration = 0;
            //loop(iteration,0,100)
            for (int iteration = 0; iteration < 100; iteration ++)
            {
                if(triangle_count-deleted_triangles<=target_count)break;

                // update mesh once in a while
                if(iteration%5==0)
                {
                    update_mesh(iteration);
                }

                // clear dirty flag
                loopi(0,triangles.size()) triangles[i].dirty=0;

                //
                // All triangles with edges below the threshold will be removed
                //
                // The following numbers works well for most models.
                // If it does not, try to adjust the 3 parameters
                //
                double threshold = 0.000000001*pow(double(iteration+3),agressiveness);

                // target number of triangles reached ? Then break
                if ((verbose) && (iteration%5==0)) {
                    printf("iteration %d - triangles %d threshold %g\n",iteration,triangle_count-deleted_triangles, threshold);
                }

                // remove vertices & mark deleted triangles
                loopi(0,triangles.size())
                {
                    Triangle &t=triangles[i];
                    if(t.err[3]>threshold) continue;
                    if(t.deleted) continue;
                    if(t.dirty) continue;

                    loopj(0,3)if(t.err[j]<threshold)
                    {

                        int i0=t.v[ j     ]; Vertex &v0 = vertices[i0];
                        int i1=t.v[(j+1)%3]; Vertex &v1 = vertices[i1];
                        // Border check
                        if(v0.border != v1.border)  continue;

                        // Compute vertex to collapse to
                        vec3f p;
                        calculate_error(i0,i1,p);
                        deleted0.resize(v0.tcount); // normals temporarily
                        deleted1.resize(v1.tcount); // normals temporarily
                        // don't remove if flipped
                        if( flipped(p,i0,i1,v0,v1,deleted0) ) continue;

                        if( flipped(p,i1,i0,v1,v0,deleted1) ) continue;

                        if ( (t.attr & TEXCOORD) == TEXCOORD  )
                        {
                            update_uvs(i0,v0,p,deleted0);
                            update_uvs(i0,v1,p,deleted1);
                        }

                        // not flipped, so remove edge
                        v0.p=p;
                        v0.q=v1.q+v0.q;
                        int tstart=refs.size();

                        update_triangles(i0,v0,deleted0,deleted_triangles);
                        update_triangles(i0,v1,deleted1,deleted_triangles);

                        int tcount=refs.size()-tstart;

                        if(tcount<=v0.tcount)
                        {
                            // save ram
                            if(tcount)memcpy(&refs[v0.tstart],&refs[tstart],tcount*sizeof(Ref));
                        }
                        else
                            // append
                            v0.tstart=tstart;

                        v0.tcount=tcount;
                        break;
                    }
                    // done?
                    if(triangle_count-deleted_triangles<=target_count)break;
                }
            }
            // clean up mesh
            compact_mesh();
        } //simplify_mesh()

        void simplify_mesh_lossless(bool verbose=false)
        {
            // init
            loopi(0,triangles.size()) triangles[i].deleted=0;

            // main iteration loop
            int deleted_triangles=0;
            std::vector<int> deleted0,deleted1;
            int triangle_count=triangles.size();
            //int iteration = 0;
            //loop(iteration,0,100)
            for (int iteration = 0; iteration < 9999; iteration ++)
            {
                // update mesh constantly
                update_mesh(iteration);
                // clear dirty flag
                loopi(0,triangles.size()) triangles[i].dirty=0;
                //
                // All triangles with edges below the threshold will be removed
                //
                // The following numbers works well for most models.
                // If it does not, try to adjust the 3 parameters
                //
                double threshold = DBL_EPSILON; //1.0E-3 EPS;
                if (verbose) {
                    printf("lossless iteration %d\n", iteration);
                }

                // remove vertices & mark deleted triangles
                loopi(0,triangles.size())
                {
                    Triangle &t=triangles[i];
                    if(t.err[3]>threshold) continue;
                    if(t.deleted) continue;
                    if(t.dirty) continue;

                    loopj(0,3)if(t.err[j]<threshold)
                    {
                        int i0=t.v[ j     ]; Vertex &v0 = vertices[i0];
                        int i1=t.v[(j+1)%3]; Vertex &v1 = vertices[i1];

                        // Border check
                        if(v0.border != v1.border)  continue;

                        // Compute vertex to collapse to
                        vec3f p;
                        calculate_error(i0,i1,p);

                        deleted0.resize(v0.tcount); // normals temporarily
                        deleted1.resize(v1.tcount); // normals temporarily

                        // don't remove if flipped
                        if( flipped(p,i0,i1,v0,v1,deleted0) ) continue;
                        if( flipped(p,i1,i0,v1,v0,deleted1) ) continue;

                        if ( (t.attr & TEXCOORD) == TEXCOORD )
                        {
                            update_uvs(i0,v0,p,deleted0);
                            update_uvs(i0,v1,p,deleted1);
                        }

                        // not flipped, so remove edge
                        v0.p=p;
                        v0.q=v1.q+v0.q;
                        int tstart=refs.size();

                        update_triangles(i0,v0,deleted0,deleted_triangles);
                        update_triangles(i0,v1,deleted1,deleted_triangles);

                        int tcount=refs.size()-tstart;

                        if(tcount<=v0.tcount)
                        {
                            // save ram
                            if(tcount)memcpy(&refs[v0.tstart],&refs[tstart],tcount*sizeof(Ref));
                        }
                        else
                            // append
                            v0.tstart=tstart;

                        v0.tcount=tcount;
                        break;
                    }
                }
                if(deleted_triangles<=0)break;
                deleted_triangles=0;
            } //for each iteration
            // clean up mesh
            compact_mesh();
        } //simplify_mesh_lossless()


        void load_mesh(const std::vector<Eigen::Vector3f>& in_vertices, const std::vector<uint32_t>& in_indices) {
            vertices.clear();
            triangles.clear();
            for (int i = 0; i < in_vertices.size(); i++) {
                Vertex v;
                v.p = vector3{in_vertices[i].x(), in_vertices[i].y(), in_vertices[i].z()};
                vertices.push_back(v);
            }

            for (int i = 0; i < in_indices.size()/3; i++) {
                Triangle t;
                t.v[0] = in_indices[3*i];
                t.v[1] = in_indices[3*i+1];
                t.v[2] = in_indices[3*i+2];
                t.attr = 0;
                triangles.push_back(t);
            }
        }

        void export_mesh(std::vector<Eigen::Vector3f>& in_vertices, std::vector<uint32_t>& in_indices) {
            in_vertices.clear();
            in_indices.clear();
            in_vertices.reserve(vertices.size());
            in_indices.reserve(triangles.size() * 3);
            for (auto& vertex: vertices) {
                in_vertices.push_back(Eigen::Vector3f(vertex.p.x, vertex.p.y, vertex.p.z));
            }
            for (auto& triangle: triangles) {
                in_indices.push_back(triangle.v[0]);
                in_indices.push_back(triangle.v[1]);
                in_indices.push_back(triangle.v[2]);
            }
        }
    };
    ///////////////////////////////////////////
}


