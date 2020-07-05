// Shows SMPL-X model
#include <cstdint>
#include <iostream>

#include "smpl/smpl.hpp"
#include "smpl/util.hpp"
#include "meshview/viewer.hpp"

int32_t main(int argc, char** argv) {
    if (argc > 2) {
        std::cout << "Expected <= 1 argument: SMPL-X model gender\n"
            "Will load data/models/smplx/SMPLX_[arg].npz\n"
            "(default NEUTRAL)";
        return 1;
    }
    std::string model_name = argc > 1 ? argv[1] : "NEUTRAL";
    for (auto& c : model_name) c = std::toupper(c);

    using namespace meshview;

    std::cout << "Load SMPLX_" << model_name << "\n";
    smpl::Model model(smpl::util::find_data_file("models/smplx/SMPLX_" + model_name + ".npz"),
                      smpl::util::find_data_file("models/smplx/uv.txt"));
    smpl::Body body(model);
    body.update();

    // body.save_obj("test.obj");
    Viewer viewer;
    viewer.title = "smplx++ | meshview";

    Mesh mesh(model.n_verts, model.n_faces);
    mesh.verts_pos() = body.verts;
    mesh.faces = model.faces;
    viewer.add(mesh).estimate_normals().set_shininess(4.f)
          .add_texture_solid<Texture::TYPE_DIFFUSE>(1.f, 0.7f, 0.8f)
          .add_texture_solid<Texture::TYPE_SPECULAR>(0.1f, 0.1f, 0.1f);

    // Crappy joint visualization
    // for (size_t i = 0; i < model.n_joints; ++i) {
    //     Mesh cube = Mesh::Cube(0.02f);
    //     viewer.add(std::move(cube))
    //         .set_shininess(32.f)
    //         .add_texture_solid<Texture::TYPE_DIFFUSE>(0.7f, 0.8f, 0.8f)
    //         .translate(body.joints.row(i).transpose());
    // }
    viewer.show();

    return 0;
}