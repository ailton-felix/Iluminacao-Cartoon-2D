#ifndef _MYDRAWABLE_H
#define _MYDRAWABLE_H

#ifdef __cplusplus


class MyDrawable{
	private:
	protected:
		float fColor[3];
		float bColor[3];
		float alpha;
		bool hidden;
	public:
		
		MyDrawable();
		void setAlpha(float a);
		void setFColor(float r, float g, float b);
		void setBColor(float r, float g, float b);
		virtual void show();
		virtual void hide();
		virtual void draw() = 0;
		virtual void clear(); 
		virtual void update();
		virtual ~MyDrawable();
};

#endif
#endif
