from django.contrib import admin

from .models import Choice, Question


class ChoiceInline(admin.TabularInline):
    model = Choice
    extra = 3


class QuestionAdmin(admin.ModelAdmin):
    fieldsets = [
        ('Запитання',{'fields': ['question_text']}),
        ('Опис', {'fields': ['desp_text']}),
        ('Інформація про дату', {'fields': ['pub_date'], 'classes': ['collapse']}),
    ]
    inlines = [ChoiceInline]
    list_filter = ['pub_date']
    search_fields = ['question_text']
    list_display = ('question_text', 'desp_text', 'pub_date', 'was_published_recently')

admin.site.register(Question, QuestionAdmin)