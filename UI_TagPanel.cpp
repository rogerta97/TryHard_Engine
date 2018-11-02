#include "UI_TagPanel.h"
#include "Application.h"

UI_TagPanel::UI_TagPanel()
{
}


UI_TagPanel::~UI_TagPanel()
{
}

bool UI_TagPanel::Start()
{
	show = false;
	tag_to_add = "Insert Tag Name"; 

	return true;
}

bool UI_TagPanel::Update()
{
	if (ImGui::Begin("Tags", &show))
	{
		if (!tag_list.empty())
		{
			if(ImGui::TreeNode("Added Tags"))
			{
				ImGui::Spacing(); 

				int count = 1;
				for (auto it = tag_list.begin(); it != tag_list.end(); it++)
				{
					string str = to_string(count) + ". ";
					str += (*it).c_str();

					if (ImGui::Selectable(str.c_str()))
					{
						//Delete the number 
						str = str.substr(3, str.size() - 3); 
						App->scene->GetSelectedGameObject()->SetTag(str.c_str());
						show = false;
						break;
					}

					count++;
				}

				ImGui::TreePop();
			}
			
		}
		else
		{
			ImGui::Text("*No tags*");
		}

	//	SEPARATE_WITH_SPACE

		ImGui::InputText("", (char*)tag_to_add.c_str(), tag_to_add.size()); 
		ImGui::SameLine();

		if (ImGui::Button("Add Tag"))
		{
			AddTag(tag_to_add.c_str());
		}
	
		ImGui::End();
	}

	return true;
}

void UI_TagPanel::AddTag(string new_tag)
{
	tag_list.push_back(new_tag);
}

string UI_TagPanel::GetTagByIndex(int index)
{
	string ret = "";

	int i = 0;
	for (auto it = tag_list.begin(); it != tag_list.end(); it++)
	{
		if (i == index)
		{
			ret = (*it);
			break; 
		}
	
		i++; 
	}

	return ret;
}


