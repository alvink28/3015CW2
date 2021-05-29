#include <sstream>

#include "scenebasic_uniform.h"
#include "helper/texture.h"
#include "helper/noisetex.h"
#include <iostream>
using std::cerr;
using std::endl;

#include <glm/gtc/matrix_transform.hpp>
using glm::vec3;
using glm::mat4;

SceneBasic_Uniform::SceneBasic_Uniform() : time(0), plane(13.0f, 10.0f, 200, 200)
{
    //mesh = ObjMesh::load("media/trophy.obj", true);
}


void SceneBasic_Uniform::initScene()
{    
    //noise
    compile();
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    projection = mat4(1.0f); 

    GLfloat verts[] = {
        -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 
        -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, -1.0f, 1.0f, 0.0f
    };
    GLfloat tc[] = {
        0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 
        0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f
    };

    // Set up the buffers 
    unsigned int handle[2]; 
    glGenBuffers(2, handle);

    glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
    glBufferData(GL_ARRAY_BUFFER, 6 * 3 * sizeof(float), verts, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
    glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), tc, GL_STATIC_DRAW);

    // Set up the vertex array object 
    glGenVertexArrays(1, &quad); 
    glBindVertexArray(quad);

    glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
    glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0))); 
    glEnableVertexAttribArray(0); // Vertex position

    glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
    glVertexAttribPointer((GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0))); 
    glEnableVertexAttribArray(2); // Texture coordinates

    glBindVertexArray(0); 

    prog.setUniform("NoiseTex", 0);

    GLuint noiseTex = NoiseTex::generate2DTex(6.0f); 
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, noiseTex);

    //texture
    compile();
	glEnable(GL_DEPTH_TEST);
   
    view = glm::lookAt(vec3(3.0f, 1.0f, 1.5f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
    projection = mat4(1.0f);

    float x, z;
    for (int i = 0; i < 3; i++)
    {
        std::stringstream name;
        name << "lights[" << i << "].Position";
        x = 2.0f * cosf((glm::two_pi<float>() / 3) * i);
        z = 2.0f * sinf((glm::two_pi<float>() / 3) * i);
        prog.setUniform(name.str().c_str(), view * glm::vec4(x, 1.2f, z +
            1.0f, 1.0f));
    }

    prog.setUniform("lights[0].L", vec3(0.0f, 0.0f, 0.8f));
    prog.setUniform("lights[1].L", vec3(0.0f, 0.8f, 0.0f));
    prog.setUniform("lights[2].L", vec3(0.8f, 0.0f, 0.0f));
    prog.setUniform("lights[0].La", vec3(0.0f, 0.0f, 0.8f));
    prog.setUniform("lights[1].La", vec3(0.0f, 0.8f, 0.0f));
    prog.setUniform("lights[2].La", vec3(0.8f, 0.0f, 0.0f));

    GLuint texID =
    Texture::loadTexture("media/texture/tile.jpg");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texID);

    //initialise the model matrix
    model = mat4(1.0f);

    projection = mat4(1.0f);

    prog.setUniform("Material.Kd", 0.2f, 0.55f, 0.9f); //seting the Kd uniform
    prog.setUniform("Light.Ld", 1.0f, 1.0f, 1.0f);     //setting the Ld uniform
    prog.setUniform("Light.Position", view * glm::vec4(5.0f, 5.0f, 2.0f, 0.0f)); //setting Light Position
    prog.setUniform("Light.Intensity", vec3(1.0f, 1.0f, 1.0f));
    angle = glm::half_pi<float>();

    //Vertex animation
    compile();

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    prog.setUniform("Light.Intensity", vec3(1.0f, 1.0f, 1.0f));
    angle = glm::half_pi<float>();
}

void SceneBasic_Uniform::compile()
{
	try {
		prog.compileShader("shader/basic_uniform.vert");
		prog.compileShader("shader/basic_uniform.frag");
		prog.link();
		prog.use();
	} catch (GLSLProgramException &e) {
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}
}

void SceneBasic_Uniform::update( float t )
{
    time = t;
}

void SceneBasic_Uniform::render()
{
    //animatioon
    prog.setUniform("Time", time);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    view = glm::lookAt(vec3(10.0f * cos(angle), 4.0f, 10.0f * sin(angle)),
            vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
    projection = glm::perspective(glm::radians(60.0f), (float)width / height, 0.3f, 100.0f);

    prog.setUniform("Material.Kd", 0.2f, 0.5f, 0.9f); 
    prog.setUniform("Material.Ks", 0.8f, 0.8f, 0.8f);
    prog.setUniform("Material.Ka", 0.2f, 0.5f, 0.9f);
    prog.setUniform("Material.Shininess", 100.0f);
    model = mat4(1.0f);
    model = glm::rotate(model, glm::radians(-10.0f), vec3(0.0f, 0.0f, 1.0f));
    model = glm::rotate(model, glm::radians(50.0f), vec3(1.0f, 0.0f, 0.0f));
    setMatrices(); 
    plane.render();	  
       


    view = mat4(1.0);
    drawScene();
    glFinish();


    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //prog.setUniform("Material.Kd", 0.4f, 0.4f, 0.4f);
    //prog.setUniform("Material.Ks", 0.9f, 0.9f, 0.9f);
    //prog.setUniform("Material.Ka", 0.5f, 0.5f, 0.5f);
    //prog.setUniform("Material.Shininess", 100.0f);

    //model = mat4(1.0f);
    //model = glm::rotate(model, glm::radians(0.0f), vec3(90.0f, 0.1f, 0.0f));
    //setMatrices();
    //mesh->render();

    //prog.setUniform("Material.Kd", 0.1f, 0.1f, 0.1f);
    //prog.setUniform("Material.Ks", 0.9f, 0.9f, 0.9f);
    //prog.setUniform("Material.Ka", 0.1f, 0.1f, 0.1f);
    //prog.setUniform("Material.Shininess", 180.0f);

    //model = mat4(1.0f);
    //model = glm::translate(model, vec3(0.0f, -0.45f, 0.0f));
    //setMatrices();
    //plane.render();

}

void SceneBasic_Uniform::drawScene()
{
    model = mat4(1.0f);
    setMatrices();

    glBindVertexArray(quad);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void SceneBasic_Uniform::setMatrices()
{
    mat4 mv = view * model; //model view matrix
    
    prog.setUniform("ModelViewMatrix", mv); //set the uniform for the model view matrix
    
    prog.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2]))); //we set the uniform for normal matrix
    
    prog.setUniform("MVP", projection * mv); //we set the model view matrix by multiplying the mv with the projection matrix
}

void SceneBasic_Uniform::resize(int w, int h)
{
    glViewport(0, 0, w, h);
    width = w;
    height = h;
    projection = glm::perspective(glm::radians(90.0f), (float)w / h, 0.3f, 100.0f);
}
