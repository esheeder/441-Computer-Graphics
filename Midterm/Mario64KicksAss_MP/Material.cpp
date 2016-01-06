#include "Material.h"

#include "common.h"
#include "Color.h"

Material::Material(const Color& ambient,
                   const Color& diffuse,
                   const Color& specular,
                   float shininess):
    ambient(ambient),
    diffuse(diffuse),
    specular(specular),
    shininess(shininess)
{
}

void Material::apply()
{
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient.getRgba().data());
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse.getRgba().data());
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular.getRgba().data());
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
}
