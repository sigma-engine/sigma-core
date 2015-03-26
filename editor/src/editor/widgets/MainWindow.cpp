#include <editor/widgets/MainWindow.hpp>
#include <editor/widgets/OpenGLWidget.hpp>
#include "ui_MainWindow.h"

#include <graphics/static_mesh.hpp>

#include <QFileDialog>

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

namespace sigmafive {
    namespace editor {
        namespace widgets {
            MainWindow::MainWindow(QWidget *parent) :
                    QMainWindow(parent),
                    ui(new Ui::MainWindow) {
                ui->setupUi(this);
                this->setCentralWidget(new OpenGLWidget(resource_manager_,scene_,this));
            }

            MainWindow::~MainWindow() {
                delete ui;
            }

            void MainWindow::on_actionImport_triggered() {
                std::string file = QFileDialog::getOpenFileName(this,"Import","/home/aaron/Desktop/sigma-five/resources").toStdString();
                Assimp::Importer importer;
                const aiScene *scene =  importer.ReadFile(file.c_str(),aiProcessPreset_TargetRealtime_MaxQuality |
                                                                       aiProcess_CalcTangentSpace |
                                                                       aiProcess_PreTransformVertices |
                                                                       aiProcess_OptimizeGraph |
                                                                       aiProcess_Triangulate);
                assert(scene != nullptr);
                for(unsigned int i=0;i<scene->mNumMeshes;++i) {
                    auto mesh = scene->mMeshes[i];
                    assert(mesh->HasPositions());
                    assert(mesh->HasNormals());
                    assert(mesh->HasTextureCoords(0));
                    assert(mesh->HasTangentsAndBitangents());

                    std::vector<graphics::static_mesh::vertex> vertices(mesh->mNumVertices);
                    std::vector<graphics::static_mesh::triangle> triangles(mesh->mNumFaces);

                    for(unsigned int i=0;i<mesh->mNumVertices;++i) {
                        auto pos = mesh->mVertices[i];
                        auto nor = mesh->mNormals[i];
                        auto tan = mesh->mTangents[i];
                        auto tex = mesh->mTangents[i];
                        vertices[i].position = float3(pos.x,pos.y,pos.z);
                        vertices[i].normal = float3(nor.x,nor.y,nor.z);
                        vertices[i].tangent = float3(tan.x,tan.y,tan.z);
                        vertices[i].texcoord = float2(tex.x,tex.y);
                    }

                    for(unsigned int i=0;i<mesh->mNumFaces;++i) {
                        aiFace f = mesh->mFaces[i];
                        for(unsigned int j=0;j<3;++j) {
                            triangles[i][j] = f.mIndices[j];
                        }
                    }

                    auto smesh = new graphics::static_mesh();
                    smesh->set_data(vertices,triangles);

                    auto meshid = resource_manager_.generate_key();

                    resource_manager_.insert(meshid,boost::shared_ptr<graphics::static_mesh>(smesh));
                }
            }

        }
    }
}
