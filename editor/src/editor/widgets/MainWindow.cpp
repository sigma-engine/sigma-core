#include <editor/widgets/MainWindow.hpp>
#include <editor/widgets/OpenGLWidget.hpp>
#include "ui_MainWindow.h"

#include <graphics/static_mesh.hpp>

#include <QFileDialog>

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <game/static_mesh_component_system.hpp>

namespace sigmafive {
    namespace editor {
        namespace widgets {
            MainWindow::MainWindow(QWidget *parent) :
                    QMainWindow(parent),
                    ui(new Ui::MainWindow) {
                ui->setupUi(this);
                this->setCentralWidget(new OpenGLWidget(resource_manager_,scene_,this));

                component_system_manager.add_component_system<game::static_mesh_component_system>();
            }

            MainWindow::~MainWindow() {
                delete ui;
            }

            void MainWindow::on_actionImport_triggered() {
                std::string file = QFileDialog::getOpenFileName(this,"Import","/home/aaron/Desktop/sigma-five/resources").toStdString();
                Assimp::Importer importer;
                const aiScene *scene =  importer.ReadFile(file.c_str(),aiProcess_CalcTangentSpace |
                                                                       aiProcess_JoinIdenticalVertices |
                                                                       //aiProcess_MakeLeftHanded |
                                                                       aiProcess_Triangulate |
                                                                       //aiProcess_RemoveComponent |
                                                                       //???aiProcess_GenNormals |
                                                                       //???aiProcess_GenSmoothNormals |
                                                                       //aiProcess_SplitLargeMeshes |
                                                                       //aiProcess_PreTransformVertices |
                                                                       aiProcess_LimitBoneWeights |
                                                                       aiProcess_ValidateDataStructure |
                                                                       aiProcess_ImproveCacheLocality |
                                                                       //??aiProcess_RemoveRedundantMaterials |
                                                                       aiProcess_FixInfacingNormals | //???
                                                                       aiProcess_SortByPType |
                                                                       aiProcess_FindDegenerates |
                                                                       aiProcess_FindInvalidData |
                                                                       aiProcess_GenUVCoords |
                                                                       aiProcess_TransformUVCoords | //???
                                                                       aiProcess_FindInstances |
                                                                       //aiProcess_OptimizeMeshes  |
                                                                       //aiProcess_OptimizeGraph  |
                                                                       //aiProcess_FlipUVs |
                                                                       //aiProcess_FlipWindingOrder  |
                                                                       //aiProcess_SplitByBoneCount  |
                                                                       aiProcess_Debone //???
                                                        );
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

                    auto static_mesh_uuid = resource_manager_.generate_key();
                    boost::shared_ptr<graphics::static_mesh> static_mesh(new graphics::static_mesh());
                    static_mesh->set_data(vertices,triangles);
                    resource_manager_.insert(static_mesh_uuid,std::move(static_mesh));

                    auto e = entity_manager.create();
                    component_manager.add_component<game::transform_component>(e);
                    auto static_mesh_component = component_manager.add_component<game::static_mesh_component>(e);
                    static_mesh_component->static_mesh = static_mesh_uuid;
                }
            }

        }
    }
}
