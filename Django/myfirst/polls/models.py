import datetime
import sys

from django.db import models
from django.utils import timezone

class Question(models.Model):
    question_text = models.CharField('Запитання',max_length=200)
    desp_text = models.CharField('Опис',max_length=500)
    pub_date = models.DateTimeField('дата публікації')

    def was_published_recently(self):
        return self.pub_date >= (timezone.localtime(timezone.now()) - datetime.timedelta(days=1))

    def __str__(self):
        return self.question_text

    was_published_recently.admin_order_field = 'pub_date'
    was_published_recently.boolean = True
    was_published_recently.short_description = 'Опубліковано нещодавно'

    class Meta:
        verbose_name = 'опитування'
        verbose_name_plural = 'опитування'

class Choice(models.Model):
    question = models.ForeignKey(Question, on_delete=models.CASCADE)
    choice_text = models.CharField('ВИБІР ТЕКСТУ',max_length=200)
    votes = models.IntegerField('ГОЛОС',default=0)
    def __str__(self):
        return self.choice_text
    class Meta:
        verbose_name = 'варіант'
        verbose_name_plural = 'варіанти'
