#include "svModelElement.h"

svModelElement::svModelElement()
    : m_Type("")
    , m_WholeVtkPolyData(NULL)
{
}

svModelElement::svModelElement(const svModelElement &other)
    : m_Type(other.m_Type)
    , m_SegNames(other.m_SegNames)
{
    int faceNum=other.m_Faces.size();
    m_Faces.resize(faceNum);

    for(int i=0;i<faceNum;i++)
    {
        svFace* face=new svFace;
        face->id=other.m_Faces[i]->id;
        face->name=other.m_Faces[i]->name;
        face->type=other.m_Faces[i]->type;
        face->selected=other.m_Faces[i]->selected;
        face->visible=other.m_Faces[i]->visible;
        face->opacity=other.m_Faces[i]->opacity;
        face->color[0]=other.m_Faces[i]->color[0];
        face->color[1]=other.m_Faces[i]->color[1];
        face->color[2]=other.m_Faces[i]->color[2];

        vtkSmartPointer<vtkPolyData> vpd=NULL;
        if(other.m_Faces[i]->vpd)
        {
            vpd=vtkSmartPointer<vtkPolyData>::New();
            vpd->DeepCopy(other.m_Faces[i]->vpd);
        }
        face->vpd=vpd;

        m_Faces[i]=face;
    }

    m_WholeVtkPolyData=NULL;
    if(other.m_WholeVtkPolyData)
    {
        m_WholeVtkPolyData=vtkSmartPointer<vtkPolyData>::New();
        m_WholeVtkPolyData->DeepCopy(other.m_WholeVtkPolyData);
    }

    for(int i=0;i<other.m_BlendRadii.size();i++)
    {
        m_BlendRadii.push_back(new svBlendParamRadius(*(other.m_BlendRadii[i])));
    }
}

svModelElement::~svModelElement()
{
    int faceNum=m_Faces.size();
    for(int i=0;i<faceNum;i++)
    {
        if(m_Faces[i])
        {
//            if(m_Faces[i]->vpd)
//            {
//                m_Faces[i]->vpd->Delete();
//            }

            delete m_Faces[i];
        }
    }

//    if(m_WholeVtkPolyData)
//    {
//        m_WholeVtkPolyData->Delete();
//    }
}

svModelElement* svModelElement::Clone()
{
    return new svModelElement(*this);
}

std::string svModelElement::GetType() const
{
    return m_Type;
}

std::vector<std::string> svModelElement::GetSegNames() const
{
    return m_SegNames;
}

void svModelElement::SetSegNames(std::vector<std::string> segNames)
{
    m_SegNames=segNames;
}

bool svModelElement::HasSeg(std::string segName)
{
    for(int i=0;i<m_SegNames.size();i++)
    {
        if(segName==m_SegNames[i])
            return true;
    }
    return false;
}

std::vector<svModelElement::svFace*> svModelElement::GetFaces() const
{
    return m_Faces;
}

void svModelElement::SetFaces(std::vector<svModelElement::svFace*> faces)
{
    m_Faces=faces;
}

svModelElement::svFace* svModelElement::GetFace(int id) const
{
    int idx=GetFaceIndex(id);
    if(idx<0)
        return NULL;
    else
        return m_Faces[idx];
}

svModelElement::svFace* svModelElement::GetFace(std::string name) const
{
    return GetFace(GetFaceID(name));
}

int svModelElement::GetFaceIndex(int id) const
{
    for(int i=0;i<m_Faces.size();i++)
    {
        if(m_Faces[i]&&m_Faces[i]->id==id)
            return i;
    }

    return -1;
}

std::string svModelElement::GetFaceName(int id) const
{
    svFace* face=GetFace(id);
    if(face)
        return face->name;
    else
        return "";
}

void svModelElement::SetFaceName(std::string name, int id)
{
    int index=GetFaceIndex(id);
    if(index>-1)
        m_Faces[index]->name=name;
}

vtkSmartPointer<vtkPolyData> svModelElement::GetWholeVtkPolyData() const
{
    return m_WholeVtkPolyData;
}

void svModelElement::SetWholeVtkPolyData(vtkSmartPointer<vtkPolyData> wvpd)
{
    m_WholeVtkPolyData=wvpd;
}

//int svModelElement::GetSelectedFaceIndex()
//{
//    return m_SelectedFaceIndex;
//}

void svModelElement::SetSelectedFaceIndex(int idx)
{
    if(idx>-1&&idx<m_Faces.size())
    {
        if(m_Faces[idx])
            m_Faces[idx]->selected=true;
    }

}

void svModelElement::ClearFaceSelection()
{
    for(int i=0;i<m_Faces.size();i++)
    {
        if(m_Faces[i])
            m_Faces[i]->selected=false;
    }

}

void svModelElement::SetSelectedFace(int id)
{
    svFace* face=GetFace(id);
    if(face)
        face->selected=true;
}

void svModelElement::SetSelectedFace(std::string name)
{
    svFace* face=GetFace(name);
    if(face)
        face->selected=true;
}

int svModelElement::GetFaceID(std::string name) const
{
    for(int i=0;i<m_Faces.size();i++)
    {
        if(m_Faces[i]&&m_Faces[i]->name==name)
            return m_Faces[i]->id;
    }

    return -1;
}

bool svModelElement::IsFaceSelected(std::string name)
{
    return GetFace(name)&&GetFace(name)->selected;
}

bool svModelElement::IsFaceSelected(int id)
{
    return GetFace(id)&&GetFace(id)->selected;
}

void svModelElement::CalculateBoundingBox(double *bounds)
{
    bounds[0]=0;
    bounds[1]=0;
    bounds[2]=0;
    bounds[3]=0;
    bounds[4]=0;
    bounds[5]=0;

    if (m_WholeVtkPolyData != nullptr && m_WholeVtkPolyData->GetNumberOfPoints() > 0)
    {
      m_WholeVtkPolyData->ComputeBounds();
      m_WholeVtkPolyData->GetBounds(bounds);
    }

}

std::vector<svModelElement::svBlendParamRadius*> svModelElement::GetBlendRadii()
{
    return m_BlendRadii;
}

void svModelElement::SetBlendRadii(std::vector<svModelElement::svBlendParamRadius*> blendRadii)
{
    m_BlendRadii=blendRadii;
}

//double svModelElement::GetBlendRadius(int faceID1, int faceID2)
//{
//    for(int i=0;i<m_BlendRadii.size();i++)
//    {
//        if(m_BlendRadii[i] && m_BlendRadii[i]->faceID1==faceID1 &&  m_BlendRadii[i]->faceID2==faceID2)
//            return m_BlendRadii[i]->radius;
//    }

//    return -1;
//}

svModelElement::svBlendParamRadius* svModelElement::GetBlendParamRadius(int faceID1, int faceID2)
{
    for(int i=0;i<m_BlendRadii.size();i++)
    {
        if(m_BlendRadii[i] && m_BlendRadii[i]->faceID1==faceID1 &&  m_BlendRadii[i]->faceID2==faceID2)
            return m_BlendRadii[i];
    }

    return NULL;
}
